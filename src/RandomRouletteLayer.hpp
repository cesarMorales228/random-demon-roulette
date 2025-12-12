#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class RandomRouletteLayer : public Popup<> {
protected:
    bool setup() override;
    
    int m_currentLevelID;
    CCLabelBMFont* m_levelNameLabel;
    CCLabelBMFont* m_diffLabel;
    CCLabelBMFont* m_percentLabel;
    
    void onPlay(CCObject* sender);
    void onNext(CCObject* sender);
    void onReset(CCObject* sender);
    
    void updateUI();

public:
    static RandomRouletteLayer* create();
};
