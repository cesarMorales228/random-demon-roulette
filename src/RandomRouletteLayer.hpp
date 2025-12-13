#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct RecommendedLevel;

class RandomRouletteLayer : public Popup<> {
protected:
    bool setup() override;
    
    void createLevelCell(RecommendedLevel level, float yPos, CCMenu* menu);
    void onPlayLevel(CCObject*);
    
public:
    static RandomRouletteLayer* create();
};
