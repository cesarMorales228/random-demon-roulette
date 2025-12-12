#include "RandomRouletteManager.hpp"

RandomRouletteManager* RandomRouletteManager::sharedState() {
    static auto instance = new RandomRouletteManager();
    return instance;
}

void RandomRouletteManager::init() {
    setupDemonList();
    
    // Initialize RNG once
    std::random_device rd;
    m_rng = std::mt19937(rd());
    
    loadData();
}

void RandomRouletteManager::setupDemonList() {
    // Hardcoded ~20 demons
    m_demonList = {
        {10565740, "Bloodbath", "Extreme Demon"},
        {39669528, "Kenos", "Extreme Demon"},
        {4284013,  "Cataclysm", "Extreme Demon"},
        {10651760, "Sonic Wave", "Extreme Demon"},
        {61765103, "The Golden", "Extreme Demon"},
        {56586326, "Tartarus", "Extreme Demon"},
        {81014878, "Acheron", "Extreme Demon"},
        {76142750, "Slaughterhouse", "Extreme Demon"},
        {56165243, "Zodiac", "Extreme Demon"},
        {22910793, "Sakupen Hell", "Extreme Demon"},
        {3633830,  "The Nightmare", "Easy Demon"},
        {3307584,  "The Lightning Road", "Easy Demon"},
        {122607,   "Demon Mixed", "Easy Demon"},
        {8660411,  "Platinum Adventure", "Easy Demon"},
        {422209,   "Impulse", "Medium Demon"},
        {192994,   "Crescendo", "Medium Demon"},
        {1111,     "xStep v2", "Medium Demon"},
        {7828189,  "Speed Racer", "Easy Demon"},
        {191199,   "RadioActive", "Medium Demon"},
        {97486,    "Clubstep v2", "Medium Demon"}
    };
}

void RandomRouletteManager::loadData() {
    // Current Level
    m_currentLevelID = Mod::get()->getSavedValue<int>("current_level_id", -1);

    // Progress Map
    // Mod::get()->getSavedValue supports std::map<int, int>
    m_levelProgress = Mod::get()->getSavedValue<std::map<int, int>>("level_progress", {});

    // Completed Levels
    // std::set might not be directly supported by older Geode versions serialization, 
    // keeping it simple with std::vector for save if needed or check docs.
    // Assuming vector for save, converting to set for runtime for O(1) checks.
    auto completedVec = Mod::get()->getSavedValue<std::vector<int>>("completed_levels", {});
    for (int id : completedVec) {
        m_completedLevels.insert(id);
    }

    log::info("RandomRoulette: Data loaded. Current Level: {}, Completed: {}", m_currentLevelID, m_completedLevels.size());
}

void RandomRouletteManager::saveData() {
    Mod::get()->setSavedValue("current_level_id", m_currentLevelID);
    Mod::get()->setSavedValue("level_progress", m_levelProgress);
    
    // Convert set back to vector for safe generic serialization
    std::vector<int> completedVec(m_completedLevels.begin(), m_completedLevels.end());
    Mod::get()->setSavedValue("completed_levels", completedVec);
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
    int randomIndex = distrib(m_rng);
    int newID = candidates[randomIndex];

    m_currentLevelID = newID;
    saveData();
    
    log::info("RandomRoulette: Picked level {}", newID);
    return newID;
}

void RandomRouletteManager::markLevelCompleted(int levelID) {
    if (m_completedLevels.find(levelID) == m_completedLevels.end()) {
        m_completedLevels.insert(levelID);
        // Also ensure progress is 100%
        m_levelProgress[levelID] = 100;
        saveData();
        log::info("RandomRoulette: Level {} marked completed.", levelID);
    }
}

int RandomRouletteManager::getBestProgress(int levelID) const {
    auto it = m_levelProgress.find(levelID);
    if (it != m_levelProgress.end()) {
        return it->second;
    }
    return 0;
}

void RandomRouletteManager::updateProgress(int levelID, int percent) {
    int oldPercent = getBestProgress(levelID);
    if (percent > oldPercent) {
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
    log::info("RandomRoulette: Reset all progress.");
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
    return {-1, "Unknown", "Unknown"};
}
