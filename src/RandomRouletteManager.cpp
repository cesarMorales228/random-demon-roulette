#include "RandomRouletteManager.hpp"

RandomRouletteManager* RandomRouletteManager::getInstance() {
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
    // 20 Hardcoded Levels
    m_demonList = {
        {10565740, "Bloodbath", DemonLevel::Difficulty::Extreme},
        {42584142, "Tartarus", DemonLevel::Difficulty::Extreme},
        {77151846, "Slaughterhouse", DemonLevel::Difficulty::Extreme},
        {61079355, "Acu", DemonLevel::Difficulty::Extreme},
        {3979721,  "Cataclysm", DemonLevel::Difficulty::Extreme},
        {97486,    "Clubstep v2", DemonLevel::Difficulty::Medium},
        {1111,     "xStep v2", DemonLevel::Difficulty::Medium},
        {191199,   "RadioActive", DemonLevel::Difficulty::Medium},
        {422209,   "Impulse", DemonLevel::Difficulty::Medium},
        {192994,   "Crescendo", DemonLevel::Difficulty::Medium},
        {8660411,  "Platinum Adventure", DemonLevel::Difficulty::Easy},
        {122607,   "Demon Mixed", DemonLevel::Difficulty::Easy},
        {3307584,  "The Lightning Road", DemonLevel::Difficulty::Easy},
        {3633830,  "The Nightmare", DemonLevel::Difficulty::Easy},
        {7828189,  "Speed Racer", DemonLevel::Difficulty::Easy},
        {56165243, "Zodiac", DemonLevel::Difficulty::Extreme},
        {61765103, "The Golden", DemonLevel::Difficulty::Extreme},
        {10651760, "Sonic Wave", DemonLevel::Difficulty::Extreme},
        {39669528, "Kenos", DemonLevel::Difficulty::Extreme},
        {81014878, "Acheron", DemonLevel::Difficulty::Extreme}
    };
}

void RandomRouletteManager::loadData() {
    auto mod = Mod::get();
    m_currentLevelIndex = mod->getSavedValue<int>("current_level_id", 0);
    
    // Load Progress Map
    try {
        auto progressVal = mod->getSavedValue<matjson::Value>("level_progress");
        if (progressVal.isObject()) {
            for (const auto& [key, val] : progressVal.asObject().unwrap()) {
                m_levelProgress[std::stoi(key)] = val.asInt().unwrapOr(0);
            }
        }
    } catch(...) {}
    
    // Load Completed Set
    try {
        auto completedVal = mod->getSavedValue<matjson::Value>("completed_levels");
        if (completedVal.isArray()) {
            for (const auto& item : completedVal.asArray().unwrap()) {
                m_completedLevels.insert(item.asInt().unwrapOr(0));
            }
        }
    } catch(...) {}
    
    log::info("RandomRoulette: Loaded data (Current: {})", m_currentLevelIndex);
}

void RandomRouletteManager::saveData() {
    auto mod = Mod::get();
    mod->setSavedValue("current_level_id", m_currentLevelIndex);
    
    // map<int,int> -> JSON object
    std::map<std::string, matjson::Value> progressMap;
    for (const auto& [id, percent] : m_levelProgress) {
        progressMap[std::to_string(id)] = percent;
    }
    mod->setSavedValue("level_progress", matjson::Value(progressMap));
    
    // set<int> -> JSON array
    std::vector<matjson::Value> completedVec;
    for (int id : m_completedLevels) {
        completedVec.push_back(matjson::Value(id)); // Explicit conversion to Value
    }
    mod->setSavedValue("completed_levels", matjson::Value(completedVec));
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
    m_currentLevelIndex = candidates[idx];
    
    saveData();
    log::info("RandomRoulette: Picked level {}", m_currentLevelIndex);
    return m_currentLevelIndex;
}

void RandomRouletteManager::markCompleted(int id) {
    if (m_completedLevels.find(id) == m_completedLevels.end()) {
        m_completedLevels.insert(id);
        m_levelProgress[id] = 100;
        saveData();
        log::info("RandomRoulette: Level {} completed", id);
    }
}

void RandomRouletteManager::updateProgress(int id, int percent) {
    if (percent > getBestProgress(id)) {
        m_levelProgress[id] = percent;
        saveData();
        log::info("RandomRoulette: Progress update {}% for {}", percent, id);
    }
}

int RandomRouletteManager::getBestProgress(int id) const {
    auto it = m_levelProgress.find(id);
    return (it != m_levelProgress.end()) ? it->second : 0;
}

void RandomRouletteManager::resetRoulette() {
    m_completedLevels.clear();
    m_levelProgress.clear();
    m_currentLevelIndex = 0;
    saveData();
    log::info("RandomRoulette: Reset");
}

const DemonLevel* RandomRouletteManager::getCurrentLevel() const {
    if (m_currentLevelIndex <= 0) return nullptr;
    for (const auto& level : m_demonList) {
        if (level.id == m_currentLevelIndex) return &level;
    }
    return nullptr;
}

bool RandomRouletteManager::isCompleted(int id) const {
    return m_completedLevels.find(id) != m_completedLevels.end();
}
