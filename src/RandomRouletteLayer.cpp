#include "RandomRouletteLayer.hpp"
#include "RandomRouletteManager.hpp"
#include <Geode/utils/web.hpp>

RandomRouletteLayer* RandomRouletteLayer::create() {
    auto ret = new RandomRouletteLayer();
    if (ret && ret->initAnchored(320.f, 240.f)) { // Slightly larger popup
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool RandomRouletteLayer::setup() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto mgr = RandomRouletteManager::sharedState();
    mgr->init(); // Ensure init

    // Title
    auto title = CCLabelBMFont::create("Random Demon Roulette", "goldFont.fnt");
    title->setPosition(m_mainLayer->getContentSize().width / 2, m_mainLayer->getContentSize().height - 25);
    title->setScale(0.8f);
    m_mainLayer->addChild(title);

    // Level Name
    m_levelNameLabel = CCLabelBMFont::create("Unknown", "bigFont.fnt");
    m_levelNameLabel->setPosition(m_mainLayer->getContentSize().width / 2, m_mainLayer->getContentSize().height / 2 + 30);
    m_levelNameLabel->setScale(0.6f);
    m_mainLayer->addChild(m_levelNameLabel);

    // Difficulty
    m_diffLabel = CCLabelBMFont::create("Difficulty", "chatFont.fnt");
    m_diffLabel->setPosition(m_mainLayer->getContentSize().width / 2, m_mainLayer->getContentSize().height / 2 + 5);
    m_diffLabel->setColor({255, 200, 0});
    m_mainLayer->addChild(m_diffLabel);

    // Percentage
    m_percentLabel = CCLabelBMFont::create("Best: 0%", "chatFont.fnt");
    m_percentLabel->setPosition(m_mainLayer->getContentSize().width / 2, m_mainLayer->getContentSize().height / 2 - 15);
    m_mainLayer->addChild(m_percentLabel);

    // Buttons Menu
    auto menu = CCMenu::create();
    menu->setPosition(0, 0);
    m_mainLayer->addChild(menu);

    // Play Button
    auto playSpr = ButtonSprite::create("Jugar", "goldFont.fnt", "GJ_button_01.png", .8f);
    auto playBtn = CCMenuItemSpriteExtra::create(playSpr, this, menu_selector(RandomRouletteLayer::onPlay));
    playBtn->setPosition(m_mainLayer->getContentSize().width / 2 - 80, 50);
    menu->addChild(playBtn);

    // Next Button
    auto nextSpr = ButtonSprite::create("Siguiente", "goldFont.fnt", "GJ_button_02.png", .8f);
    auto nextBtn = CCMenuItemSpriteExtra::create(nextSpr, this, menu_selector(RandomRouletteLayer::onNext));
    nextBtn->setPosition(m_mainLayer->getContentSize().width / 2 + 80, 50);
    menu->addChild(nextBtn);

    // Reset Button (Small icon)
    auto resetSpr = CCSprite::createWithSpriteFrameName("GJ_resetBtn_001.png");
    resetSpr->setScale(0.6f);
    auto resetBtn = CCMenuItemSpriteExtra::create(resetSpr, this, menu_selector(RandomRouletteLayer::onReset));
    resetBtn->setPosition(m_mainLayer->getContentSize().width - 30, m_mainLayer->getContentSize().height - 30);
    menu->addChild(resetBtn);

    updateUI();

    return true;
}

void RandomRouletteLayer::updateUI() {
    auto mgr = RandomRouletteManager::sharedState();
    m_currentLevelID = mgr->getCurrentLevelID();

    if (m_currentLevelID == -1) {
        m_levelNameLabel->setString("Pulsa Siguiente");
        m_diffLabel->setString("");
        m_percentLabel->setString("");
        return;
    }

    auto data = mgr->getLevelData(m_currentLevelID);
    m_levelNameLabel->setString(data.name.c_str());
    m_diffLabel->setString(data.difficulty.c_str());
    
    int best = mgr->getBestProgress(m_currentLevelID);
    bool completed = mgr->isCompleted(m_currentLevelID);
    
    if (completed) {
        m_percentLabel->setString("Completado!");
        m_percentLabel->setColor({0, 255, 0});
    } else {
        m_percentLabel->setString(fmt::format("Best: {}%", best).c_str());
        m_percentLabel->setColor({255, 255, 255});
    }
}

void RandomRouletteLayer::onPlay(CCObject*) {
    if (m_currentLevelID <= 0) return;
    
    // Get Online Level to play
    GameLevelManager::sharedState()->getOnlineLevel(m_currentLevelID, false);
    
    // Create a dummy level to open info immediately if we assume user has it, 
    // but better to rely on LevelInfoLayer handling it.
    auto level = GJGameLevel::create();
    level->m_levelID = m_currentLevelID;
    
    auto levelInfo = LevelInfoLayer::create(level, false);
    CCDirector::sharedDirector()->getRunningScene()->addChild(levelInfo);
    
    this->onClose(nullptr);
}

void RandomRouletteLayer::onNext(CCObject*) {
    auto mgr = RandomRouletteManager::sharedState();
    int id = mgr->pickRandomLevel();
    if (id == -1) {
        FLAlertLayer::create("Info", "Todos los niveles completados! Reinicia la ruleta.", "OK")->show();
    }
    updateUI();
}

void RandomRouletteLayer::onReset(CCObject*) {
    createQuickPopup(
        "Reiniciar",
        "¿Estás seguro de querer reiniciar todo el progreso de la ruleta?",
        "Cancelar", "Si",
        [this](auto, bool btn2) {
            if (btn2) {
                RandomRouletteManager::sharedState()->resetRoulette();
                updateUI();
            }
        }
    );
}
