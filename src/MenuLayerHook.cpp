#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "RandomRouletteLayer.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        
        // Usar runAction con CallFunc en vez de scheduleOnce
        this->runAction(CCSequence::create(
            CCDelayTime::create(0.1f),
            CCCallFunc::create(this, callfunc_selector(MyMenuLayer::addRouletteButton)),
            nullptr
        ));
        
        return true;
    }
    
    void addRouletteButton() {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        auto btnSprite = CCSprite::createWithSpriteFrameName("GJ_button_01.png");
        if (!btnSprite) {
            log::error("Failed to create button sprite");
            return;
        }
        
        auto btn = CCMenuItemSpriteExtra::create(
            btnSprite,
            this,
            menu_selector(MyMenuLayer::onRouletteButton)
        );
        
        if (!btn) {
            log::error("Failed to create button");
            return;
        }
        
        btn->setPosition(winSize.width - 30, 30);
        
        auto menu = CCMenu::create();
        menu->addChild(btn);
        menu->setPosition(0, 0);
        menu->setID("roulette-menu"_spr);
        this->addChild(menu, 10);
        
        log::info("Roulette button added successfully");
    }
    
    void onRouletteButton(CCObject*) {
        RandomRouletteLayer::create()->show();
    }
};
