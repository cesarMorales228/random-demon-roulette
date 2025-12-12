#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "RandomRouletteLayer.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID("bottom-menu");
        if (menu) {
            auto spr = CCSprite::createWithSpriteFrameName("GJ_button_01.png");
            // Add a label or icon on top if desired, or just use the button
            // Scaling it down slightly to fit commonly packed menus
            spr->setScale(0.7f);
            
            auto btn = CCMenuItemSpriteExtra::create(
                spr,
                this,
                menu_selector(MyMenuLayer::onRoulette)
            );
            btn->setID("random-roulette-btn"_spr);
            
            menu->addChild(btn);
            menu->updateLayout();
        }
        return true;
    }

    void onRoulette(CCObject*) {
        RandomRouletteLayer::create()->show();
    }
};
