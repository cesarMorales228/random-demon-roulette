#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "RandomRouletteLayer.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        
        // Esperar un frame para asegurar que todo esté inicializado
        this->scheduleOnce([this](float) {
            this->addRouletteButton();
        }, 0.0f, "add_roulette_button");
        
        return true;
    }
    
    void addRouletteButton() {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        // Crear sprite del botón
        auto btnSprite = CCSprite::createWithSpriteFrameName("GJ_button_01.png");
        if (!btnSprite) {
            log::error("Failed to create button sprite");
            return;
        }
        
        // Crear el botón
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
        
        // Crear menú nuevo
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
