#pragma once
#include <Geode/Geode.hpp>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <random>

using namespace geode::prelude;

// Struct to hold level basics (Hardcoded list)
struct DemonLevel {
    int id;
    std::string name;
    // We can store difficulty as string or rely on ID to fetch. 
    // Requirement says: "dificultad (enum: Easy/Medium/Hard/Extreme)"
    // For simplicity in hardcoded list, we'll store a string representation or enum.
    std::string difficulty; 
};

class RandomRouletteManager {
public:
    static RandomRouletteManager* sharedState();

    void init();
    
    // Core Logic
    int pickRandomLevel();
    void markLevelCompleted(int levelID);
    int getBestProgress(int levelID) const;
    void updateProgress(int levelID, int percent);
    void resetRoulette();
    
    int getCurrentLevelID() const;
    bool isCompleted(int levelID) const;
    DemonLevel getLevelData(int levelID) const;

private:
    std::vector<DemonLevel> m_demonList;
    
    // Saved Data
    int m_currentLevelID = -1;
    std::map<int, int> m_levelProgress;
    std::set<int> m_completedLevels;
    
    std::mt19937 m_rng;

    void loadData();
    void saveData();
    void setupDemonList();
};
