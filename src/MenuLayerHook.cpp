#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "RandomRouletteLayer.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto btnSprite = CCSprite::createWithSpriteFrameName("GJ_button_01.png");
        
        // Add "R" label
        auto label = CCLabelBMFont::create("R", "goldFont.fnt");
        label->setPosition(btnSprite->getContentSize() / 2);
        label->setScale(0.8f);
        btnSprite->addChild(label);
        
        auto btn = CCMenuItemSpriteExtra::create(
            btnSprite,
            this,
            menu_selector(MyMenuLayer::onRouletteButton)
        );
        // Position at top right or bottom right depending on preference, user said "bottom right" in prompt
        // "winSize.width - 30, 30" corresponds to bottom right
        btn->setPosition(winSize.width - 30, 30);
        
        auto menu = CCMenu::create();
        menu->addChild(btn);
        menu->setPosition(0, 0);
        this->addChild(menu);
        
        return true;
    }
    
    void onRouletteButton(CCObject*) {
        RandomRouletteLayer::create()->show();
    }
};
