cmake_minimum_required(VERSION 3.21)
project(random_roulette)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/RandomRouletteManager.cpp
    src/RandomRouletteManager.hpp
    src/RandomRouletteLayer.cpp
    src/RandomRouletteLayer.hpp
    src/MenuLayerHook.cpp
    src/PlayLayerHook.cpp
)

target_link_libraries(${PROJECT_NAME} geode-sdk)

setup_geode_mod(
    MOD_ID dainamondev.randomroulette
    VERSION 1.0.0
    NAME "Random Demon Roulette"
    DESCRIPTION "Roulette that picks random demons, tracks progress, and avoids repeats."
    AUTHORS "dainamondev"
    EXTERNALS
        geode.geode:>=4.0.0
)
