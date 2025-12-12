#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "RandomRouletteLayer.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID("bottom-menu");
        if (menu) {
            // Button Sprite
            auto spr = ButtonSprite::create("R", 0, false, "goldFont.fnt", "GJ_button_01.png", 30.f, 0.6f);
            
            auto btn = CCMenuItemSpriteExtra::create(
                spr,
                this,
                menu_selector(MyMenuLayer::onRoulette)
            );
            btn->setID("roulette-button"_spr);
            
            menu->addChild(btn);
            menu->updateLayout();
        }
        return true;
    }

    void onRoulette(CCObject*) {
        RandomRouletteLayer::create()->show();
    }
};
