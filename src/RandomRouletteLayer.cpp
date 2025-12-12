#include "RandomRouletteLayer.hpp"
#include "RandomRouletteManager.hpp"
#include <Geode/utils/web.hpp>

RandomRouletteLayer* RandomRouletteLayer::create() {
    auto ret = new RandomRouletteLayer();
    if (ret && ret->initAnchored(300.f, 220.f)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool RandomRouletteLayer::setup() {
    auto mgr = RandomRouletteManager::sharedState();
    mgr->init();

    this->setTitle("Random Demon Roulette");

    // Level Name
    m_nameLabel = CCLabelBMFont::create("Loading...", "bigFont.fnt");
    m_nameLabel->setScale(0.6f);
    m_nameLabel->setPosition(m_size.width / 2, m_size.height / 2 + 25);
    m_mainLayer->addChild(m_nameLabel);

    // Difficulty
    m_diffLabel = CCLabelBMFont::create("Difficulty", "chatFont.fnt");
    m_diffLabel->setPosition(m_size.width / 2, m_size.height / 2);
    m_mainLayer->addChild(m_diffLabel);

    // Progress
    m_progressLabel = CCLabelBMFont::create("Best: 0%", "chatFont.fnt");
    m_progressLabel->setPosition(m_size.width / 2, m_size.height / 2 - 20);
    m_mainLayer->addChild(m_progressLabel);

    // Buttons
    auto menu = CCMenu::create();
    menu->setPosition(0, 0);
    m_mainLayer->addChild(menu);

    // Play
    auto playSpr = ButtonSprite::create("Play", "goldFont.fnt", "GJ_button_01.png", .8f);
    auto playBtn = CCMenuItemSpriteExtra::create(playSpr, this, menu_selector(RandomRouletteLayer::onPlay));
    playBtn->setPosition(m_size.width / 2 - 80, 40);
    menu->addChild(playBtn);

    // Next
    auto nextSpr = ButtonSprite::create("Next", "goldFont.fnt", "GJ_button_02.png", .8f);
    auto nextBtn = CCMenuItemSpriteExtra::create(nextSpr, this, menu_selector(RandomRouletteLayer::onNext));
    nextBtn->setPosition(m_size.width / 2 + 80, 40);
    menu->addChild(nextBtn);

    // Reset (Corner)
    auto resetSpr = CCSprite::createWithSpriteFrameName("GJ_resetBtn_001.png");
    resetSpr->setScale(0.8f);
    auto resetBtn = CCMenuItemSpriteExtra::create(resetSpr, this, menu_selector(RandomRouletteLayer::onReset));
    resetBtn->setPosition(m_size.width - 25, m_size.height - 25);
    menu->addChild(resetBtn);

    updateUI();
    return true;
}

void RandomRouletteLayer::updateUI() {
    auto mgr = RandomRouletteManager::sharedState();
    m_currentLevelID = mgr->getCurrentLevelID();

    if (m_currentLevelID == -1) {
        m_nameLabel->setString("Click Next");
        m_diffLabel->setString("");
        m_progressLabel->setString("");
        return;
    }

    auto data = mgr->getLevelData(m_currentLevelID);
    m_nameLabel->setString(data.name.c_str());

    // Difficulty Color & Text
    ccColor3B diffColor = {255, 255, 255};
    std::string diffText = "";
    switch(data.difficulty) {
        case Difficulty::Easy: diffText = "Easy Demon"; diffColor = {0, 255, 0}; break; // Green
        case Difficulty::Medium: diffText = "Medium Demon"; diffColor = {255, 255, 0}; break; // Yellow
        case Difficulty::Hard: diffText = "Hard Demon"; diffColor = {255, 128, 0}; break; // Orange
        case Difficulty::Extreme: diffText = "Extreme Demon"; diffColor = {255, 0, 0}; break; // Red
    }
    m_diffLabel->setString(diffText.c_str());
    m_diffLabel->setColor(diffColor);

    // Progress
    if (mgr->isCompleted(m_currentLevelID)) {
        m_progressLabel->setString("Completed!");
        m_progressLabel->setColor({0, 255, 0});
    } else {
        int best = mgr->getBestProgress(m_currentLevelID);
        m_progressLabel->setString(fmt::format("Progress: {}%", best).c_str());
        m_progressLabel->setColor({255, 255, 255});
    }
}

void RandomRouletteLayer::onPlay(CCObject*) {
    if (m_currentLevelID <= 0) return;

    // Trigger Play logic
    // Using GameLevelManager to fetch implies we intend to download/open it.
    // getOnlineLevel is appropriate.
    GameLevelManager::sharedState()->getOnlineLevel(m_currentLevelID, false);
    
    // Create info layer immediately to prevent confusion
    auto level = GJGameLevel::create();
    level->m_levelID = m_currentLevelID;
    
    auto infoLayer = LevelInfoLayer::create(level, false);
    CCDirector::sharedDirector()->getRunningScene()->addChild(infoLayer);

    this->onClose(nullptr);
}

void RandomRouletteLayer::onNext(CCObject*) {
    auto mgr = RandomRouletteManager::sharedState();
    int id = mgr->pickRandomLevel();
    if (id == -1) {
        FLAlertLayer::create("Info", "All demons completed!", "OK")->show();
    }
    updateUI();
}

void RandomRouletteLayer::onReset(CCObject*) {
    createQuickPopup("Reset", "Are you sure you want to reset all progress?", "Cancel", "Reset", 
        [this](auto, bool btn2){
            if(btn2) {
                RandomRouletteManager::sharedState()->resetRoulette();
                updateUI();
            }
        });
}
