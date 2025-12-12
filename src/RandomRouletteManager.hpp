#pragma once
#include <Geode/Geode.hpp>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <random>
#include <matjson.hpp>

using namespace geode::prelude;

enum class Difficulty {
    Easy,
    Medium,
    Hard,
    Extreme
};

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
    DemonLevel getLevelData(int levelID) const;
    bool isCompleted(int levelID) const;

private:
    std::vector<DemonLevel> m_demonList;
    
    int m_currentLevelID = -1;
    std::map<int, int> m_levelProgress;
    std::set<int> m_completedLevels;
    
    std::mt19937 m_rng;

    void loadData();
    void saveData();
    void setupDemonList();
};
