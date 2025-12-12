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
    
    // Load Current Level ID
    m_currentLevelID = mod->getSavedValue<int>("current_level_id", -1);

    // Load Level Progress (JSON Object -> Map)
    // Saved format: { "ID": Percent, ... }
    m_levelProgress.clear();
    // Use try-catch or checks if older data might be corrupt? Geode handles basic type errors usually.
    // We use matjson::Value to be safe.
    if (auto val = mod->getSavedValue<matjson::Value>("level_progress")) {
        if (val.is_object()) {
            for (const auto& [key, value] : val.as_object()) {
                try {
                    int id = std::stoi(key);
                    int percent = 0;
                    if (value.is_number()) percent = value.as_int();
                    m_levelProgress[id] = percent;
                } catch(...) {}
            }
        }
    }

    // Load Completed Levels (JSON Array -> Set)
    m_completedLevels.clear();
    if (auto val = mod->getSavedValue<matjson::Value>("completed_levels")) {
        if (val.is_array()) {
            for (const auto& item : val.as_array()) {
                if (item.is_number()) {
                    m_completedLevels.insert(item.as_int());
                }
            }
        }
    }

    log::info("Roulette Loaded: Current: {}, Completed: {}", m_currentLevelID, m_completedLevels.size());
}

void RandomRouletteManager::saveData() {
    auto mod = Mod::get();
    
    mod->setSavedValue("current_level_id", m_currentLevelID);

    // Save Level Progress as JSON Object
    matjson::Value progressObj = matjson::Object();
    for (const auto& [id, percent] : m_levelProgress) {
        progressObj[std::to_string(id)] = percent;
    }
    mod->setSavedValue("level_progress", progressObj);

    // Save Completed Levels as JSON Array
    matjson::Value completedArr = matjson::Array();
    for (int id : m_completedLevels) {
        completedArr.push(id);
    }
    mod->setSavedValue("completed_levels", completedArr);
}

int RandomRouletteManager::pickRandomLevel() {
    std::vector<int> candidates;
    for (const auto& level : m_demonList) {
        if (m_completedLevels.find(level.id) == m_completedLevels.end()) {
            candidates.push_back(level.id);
        }
    }

    // If all completed
    if (candidates.empty()) {
        log::info("All levels completed!");
        return -1;
    }

    std::uniform_int_distribution<> distrib(0, candidates.size() - 1);
    int idx = distrib(m_rng);
    m_currentLevelID = candidates[idx];
    
    saveData();
    log::info("Picked new random level: {}", m_currentLevelID);
    return m_currentLevelID;
}

void RandomRouletteManager::markLevelCompleted(int levelID) {
    if (m_completedLevels.find(levelID) == m_completedLevels.end()) {
        m_completedLevels.insert(levelID);
        m_levelProgress[levelID] = 100;
        saveData();
        log::info("Level {} marked completed!", levelID);
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
        log::info("Updated progress for level {}: {}%", levelID, percent);
    }
}

void RandomRouletteManager::resetRoulette() {
    m_completedLevels.clear();
    m_levelProgress.clear();
    m_currentLevelID = -1;
    saveData();
    log::info("Roulette reset.");
}

int RandomRouletteManager::getCurrentLevelID() const {
    return m_currentLevelID;
}

bool RandomRouletteManager::isCompleted(int levelID) const {
    return m_completedLevels.find(levelID) != m_completedLevels.end();
}

DemonLevel RandomRouletteManager::getLevelData(int levelID) const {
    for (const auto& level : m_demonList) {
        if (level.id == levelID) return level;
    }
    return { -1, "Unknown", Difficulty::Easy };
}
