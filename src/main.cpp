#include <Geode/Geode.hpp>
#include "RandomRouletteManager.hpp"
#include <matjson.hpp>

using namespace geode::prelude;

$execute {
    RandomRouletteManager::getInstance()->init();
    log::info("Random Demon Roulette Initialized (Verified APIs)");
}
