#include "RandomRouletteManager.hpp"

RandomRouletteManager* RandomRouletteManager::sharedState() {
    static auto instance = new RandomRouletteManager();
    return instance;
}

void RandomRouletteManager::init() {
    setupDemonList();
    
    std::random_device rd;
    m_rng = std::mt19937(rd());
    
    loadData();
}

void RandomRouletteManager::setupDemonList() {
    m_demonList = {
        {10565740, "Bloodbath", Difficulty::Extreme},
        {39669528, "Kenos", Difficulty::Extreme},
        {4284013,  "Cataclysm", Difficulty::Extreme},
        {10651760, "Sonic Wave", Difficulty::Extreme},
        {61765103, "The Golden", Difficulty::Extreme},
        {56586326, "Tartarus", Difficulty::Extreme},
        {81014878, "Acheron", Difficulty::Extreme},
        {76142750, "Slaughterhouse", Difficulty::Extreme},
        {56165243, "Zodiac", Difficulty::Extreme},
        {22910793, "Sakupen Hell", Difficulty::Extreme},
        {3633830,  "The Nightmare", Difficulty::Easy},
        {3307584,  "The Lightning Road", Difficulty::Easy},
        {122607,   "Demon Mixed", Difficulty::Easy},
        {8660411,  "Platinum Adventure", Difficulty::Easy},
        {7828189,  "Speed Racer", Difficulty::Easy},
        {422209,   "Impulse", Difficulty::Medium},
        {192994,   "Crescendo", Difficulty::Medium},
        {1111,     "xStep v2", Difficulty::Medium},
        {191199,   "RadioActive", Difficulty::Medium},
        {97486,    "Clubstep v2", Difficulty::Medium}
    };
}

void RandomRouletteManager::loadData() {
    auto mod = Mod::get();
    
    m_currentLevelID = mod->getSavedValue<int>("current_level_id", -1); // fixed default -1

    // Map Load
    m_levelProgress.clear();
    auto progressVal = mod->getSavedValue<matjson::Value>("level_progress");
    if (progressVal.has_value() && progressVal->isObject()) {
        for (const auto& [key, val] : progressVal->asObject().unwrap()) {
            try {
                int id = std::stoi(key);
                int percent = val.asInt().unwrapOr(0);
                m_levelProgress[id] = percent;
            } catch(...) {}
        }
    }

    // Set Load
    m_completedLevels.clear();
    auto completedVal = mod->getSavedValue<matjson::Value>("completed_levels");
    if (completedVal.has_value() && completedVal->isArray()) {
        for (const auto& item : completedVal->asArray().unwrap()) {
            m_completedLevels.insert(item.asInt().unwrapOr(0));
        }
    }

    log::info("RandomRoulette: Loaded (Current: {}, Completed: {})", m_currentLevelID, m_completedLevels.size());
}

void RandomRouletteManager::saveData() {
    auto mod = Mod::get();
    
    mod->setSavedValue("current_level_id", m_currentLevelID);

    // Map -> JSON Object
    auto progressObj = matjson::makeObject({});
    for (const auto& [id, percent] : m_levelProgress) {
        progressObj[std::to_string(id)] = percent;
    }
    mod->setSavedValue("level_progress", progressObj);

    // Set -> JSON Array
    std::vector<matjson::Value> completedVec;
    for (int id : m_completedLevels) {
        completedVec.push_back(id);
    }
    mod->setSavedValue("completed_levels", matjson::Array(completedVec));
}

int RandomRouletteManager::pickRandomLevel() {
    std::vector<int> candidates;
    for (const auto& level : m_demonList) {
        if (m_completedLevels.find(level.id) == m_completedLevels.end()) {
            candidates.push_back(level.id);
        }
    }

    if (candidates.empty()) {
        log::info("RandomRoulette: All levels completed!");
        return -1;
    }

    std::uniform_int_distribution<> distrib(0, candidates.size() - 1);
    int idx = distrib(m_rng);
    m_currentLevelID = candidates[idx];
    
    saveData();
    log::info("RandomRoulette: Picked level {}", m_currentLevelID);
    return m_currentLevelID;
}

void RandomRouletteManager::markLevelCompleted(int levelID) {
    if (m_completedLevels.find(levelID) == m_completedLevels.end()) {
        m_completedLevels.insert(levelID);
        m_levelProgress[levelID] = 100;
        saveData();
        log::info("RandomRoulette: Marked level {} as completed", levelID);
    }
}

int RandomRouletteManager::getBestProgress(int levelID) const {
    auto it = m_levelProgress.find(levelID);
    return (it != m_levelProgress.end()) ? it->second : 0;
}

void RandomRouletteManager::updateProgress(int levelID, int percent) {
    int current = getBestProgress(levelID);
    if (percent > current) {
        m_levelProgress[levelID] = percent;
        saveData();
        log::info("RandomRoulette: Updated progress for {} to {}%", levelID, percent);
    }
}

void RandomRouletteManager::resetRoulette() {
    m_completedLevels.clear();
    m_levelProgress.clear();
    m_currentLevelID = -1;
    saveData();
    log::info("RandomRoulette: Reset complete");
}

int RandomRouletteManager::getCurrentLevelID() const {
    return m_currentLevelID;
}

bool RandomRouletteManager::isCompleted(int levelID) const {
    return m_completedLevels.find(levelID) != m_completedLevels.end();
}

DemonLevel RandomRouletteManager::getLevelData(int levelID) const {
    for (const auto& l : m_demonList) {
        if (l.id == levelID) return l;
    }
    return {-1, "Unknown", Difficulty::Easy};
}
