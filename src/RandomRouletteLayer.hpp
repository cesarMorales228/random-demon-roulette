#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class RandomRouletteLayer : public Popup<>, public LevelManagerDelegate {
protected:
    bool setup() override;
    
    CCLabelBMFont* m_nameLabel;
    CCLabelBMFont* m_diffLabel;
    CCLabelBMFont* m_progressLabel;
    
    void onPlay(CCObject*);
    void onNext(CCObject*);
    void onReset(CCObject*);
    
    void updateUI();
    
    // LevelManagerDelegate callbacks
    void loadLevelsFinished(cocos2d::CCArray* levels, const char* key) override;
    void loadLevelsFailed(const char* key) override;
    void loadLevelsFinished(cocos2d::CCArray* levels, const char* key, int) override {}
    void loadLevelsFailed(const char* key, int) override {}
    
public:
    static RandomRouletteLayer* create();
};
