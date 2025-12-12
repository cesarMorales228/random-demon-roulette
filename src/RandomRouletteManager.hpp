#pragma once
#include <Geode/Geode.hpp>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <random>
#include <matjson.hpp>

using namespace geode::prelude;

// Difficulty Enum
enum class Difficulty {
    Easy,
    Medium,
    Hard,
    Extreme
};

// Level Data Struct
struct DemonLevel {
    int id;
    std::string name;
    Difficulty difficulty;
};

class RandomRouletteManager {
public:
    static RandomRouletteManager* sharedState();

    void init();

    // Logic
    int pickRandomLevel();
    void markLevelCompleted(int levelID);
    int getBestProgress(int levelID) const;
    void updateProgress(int levelID, int percent);
    void resetRoulette();

    // Getters
    int getCurrentLevelID() const;
    bool isCompleted(int levelID) const;
    DemonLevel getLevelData(int levelID) const;

private:
    std::vector<DemonLevel> m_demonList;
    
    // Runtime Data
    int m_currentLevelID = -1;
    std::map<int, int> m_levelProgress;
    std::set<int> m_completedLevels;
    
    std::mt19937 m_rng;

    // Internal
    void loadData();
    void saveData();
    void setupDemonList();
};
