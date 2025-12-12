#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "RandomRouletteLayer.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        
        // Usar runAction con CallFunc para agregar el botón después de un delay
        this->runAction(CCSequence::create(
            CCDelayTime::create(0.1f),
            CCCallFunc::create(this, callfunc_selector(MyMenuLayer::addRouletteButton)),
            nullptr
        ));
        
        return true;
    }
    
    void addRouletteButton() {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        // Crear botón con ButtonSprite (que siempre funciona)
        auto btnSprite = ButtonSprite::create("Roulette", 70, true, "bigFont.fnt", "GJ_button_01.png", 0, 0.8f);
        
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
        
        // Posición en centro-abajo (más visible)
        btn->setPosition(winSize.width / 2, 50.0f);
        
        auto menu = CCMenu::create();
        menu->addChild(btn);
        menu->setPosition(0, 0);
        menu->setID("roulette-menu"_spr);
        this->addChild(menu, 100);
        
        log::info("Roulette button added successfully");
    }
    
    void onRouletteButton(CCObject*) {
        RandomRouletteLayer::create()->show();
    }
};
