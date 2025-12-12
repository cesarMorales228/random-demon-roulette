#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class RandomRouletteLayer : public Popup<> {
protected:
    bool setup() override;

    int m_currentLevelID;
    CCLabelBMFont* m_nameLabel;
    CCLabelBMFont* m_diffLabel;
    CCLabelBMFont* m_progressLabel;

    void onPlay(CCObject*);
    void onNext(CCObject*);
    void onReset(CCObject*);
    
    void updateUI();

public:
    static RandomRouletteLayer* create();
};
