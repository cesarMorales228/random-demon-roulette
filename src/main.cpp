#include <Geode/Geode.hpp>
#include "RandomRouletteManager.hpp"

using namespace geode::prelude;

$execute {
    RandomRouletteManager::sharedState()->init();
    log::info("Random Demon Roulette Initialized (Strict Mode)");
}
