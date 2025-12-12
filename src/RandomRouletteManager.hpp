#pragma once
#include <Geode/Geode.hpp>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <random>
#include <matjson.hpp>

using namespace geode::prelude;

struct DemonLevel {
    int id;
    std::string name;
    enum class Difficulty { Easy, Medium, Hard, Extreme } difficulty;
};

class RandomRouletteManager {
public:
    static RandomRouletteManager* getInstance();

    void init();

    // Logic
    int pickRandomLevel();
    void markCompleted(int id);
    void updateProgress(int id, int percent);
    int getBestProgress(int id) const;
    void resetRoulette();
    
    // Getters
    const DemonLevel* getCurrentLevel() const;
    bool isCompleted(int id) const;

private:
    std::vector<DemonLevel> m_demonList;
    
    int m_currentLevelIndex = 0; // Stores ID
    std::map<int, int> m_levelProgress;
    std::set<int> m_completedLevels;
    
    std::mt19937 m_rng;

    void loadData();
    void saveData();
    void setupDemonList();
};
