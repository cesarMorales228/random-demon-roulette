#include <Geode/Geode.hpp>
#include "RandomRouletteManager.hpp"

using namespace geode::prelude;

$execute {
    // Initialize manager on load
    RandomRouletteManager::sharedState()->init();
    log::info("Mod loaded: Random Demon Roulette");
}
