#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "RandomRouletteLayer.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto menu = this->getChildByID("bottom-menu");

        // Add button if bottom-menu exists
        if (menu) {
            // Using a die/random icon if available, otherwise a generic one
            // GJ_everyplayBtn_001.png looks a bit like a media button, let's use a simpler one
            // or maybe the "free stuff" chest icon? GJ_chestBtn_001.png
            // Let's use a standard button with a "R" text or similar if no sprite works perfectly.
            auto spr = ButtonSprite::create("R", 40, true, "goldFont.fnt", "GJ_button_01.png", 30.f, 0.8f);
            
            auto btn = CCMenuItemSpriteExtra::create(
                spr,
                this,
                menu_selector(MyMenuLayer::onRoulette)
            );
            btn->setID("random-roulette-btn"_spr);
            
            // Add to menu
            menu->addChild(btn);
            menu->updateLayout();
        }

        return true;
    }

    void onRoulette(CCObject*) {
        RandomRouletteLayer::create()->show();
    }
};
