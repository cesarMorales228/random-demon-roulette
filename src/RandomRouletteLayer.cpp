#include "RandomRouletteLayer.hpp"
#include "RandomRouletteManager.hpp"

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
    this->setTitle("Random Demon Roulette");
    
    RandomRouletteManager::getInstance()->init();
    
    // UI Elements
    m_nameLabel = CCLabelBMFont::create("...", "bigFont.fnt");
    m_nameLabel->setScale(0.6f);
    m_nameLabel->setPosition(m_size.width / 2, m_size.height / 2 + 25);
    m_mainLayer->addChild(m_nameLabel);
    
    m_diffLabel = CCLabelBMFont::create("Difficulty", "chatFont.fnt");
    m_diffLabel->setPosition(m_size.width / 2, m_size.height / 2);
    m_mainLayer->addChild(m_diffLabel);
    
    m_progressLabel = CCLabelBMFont::create("Progress: 0%", "chatFont.fnt");
    m_progressLabel->setPosition(m_size.width / 2, m_size.height / 2 - 20);
    m_mainLayer->addChild(m_progressLabel);
    
    // Buttons
    auto menu = CCMenu::create();
    menu->setPosition(0, 0);
    m_mainLayer->addChild(menu);
    
    auto playSpr = ButtonSprite::create("Play", "goldFont.fnt", "GJ_button_01.png", .8f);
    auto playBtn = CCMenuItemSpriteExtra::create(playSpr, this, menu_selector(RandomRouletteLayer::onPlay));
    playBtn->setPosition(m_size.width / 2 - 80, 40);
    menu->addChild(playBtn);
    
    auto nextSpr = ButtonSprite::create("Next", "goldFont.fnt", "GJ_button_02.png", .8f);
    auto nextBtn = CCMenuItemSpriteExtra::create(nextSpr, this, menu_selector(RandomRouletteLayer::onNext));
    nextBtn->setPosition(m_size.width / 2 + 80, 40);
    menu->addChild(nextBtn);
    
    auto resetSpr = CCSprite::createWithSpriteFrameName("GJ_resetBtn_001.png");
    resetSpr->setScale(0.8f);
    auto resetBtn = CCMenuItemSpriteExtra::create(resetSpr, this, menu_selector(RandomRouletteLayer::onReset));
    resetBtn->setPosition(m_size.width - 25, m_size.height - 25);
    menu->addChild(resetBtn);
    
    updateUI();
    
    return true;
}

void RandomRouletteLayer::updateUI() {
    auto mgr = RandomRouletteManager::getInstance();
    auto level = mgr->getCurrentLevel();
    
    if (!level) {
        m_nameLabel->setString("Click Next");
        m_diffLabel->setString("");
        m_progressLabel->setString("");
        return;
    }
    
    m_nameLabel->setString(level->name.c_str());
    
    ccColor3B col = {255, 255, 255};
    std::string txt = "";
    
    switch(level->difficulty) {
        case DemonLevel::Difficulty::Easy: 
            txt = "Easy Demon"; 
            col = {0, 255, 0}; 
            break;
        case DemonLevel::Difficulty::Medium: 
            txt = "Medium Demon"; 
            col = {255, 255, 0}; 
            break;
        case DemonLevel::Difficulty::Hard: 
            txt = "Hard Demon"; 
            col = {255, 128, 0}; 
            break;
        case DemonLevel::Difficulty::Extreme: 
            txt = "Extreme Demon"; 
            col = {255, 0, 0}; 
            break;
    }
    
    m_diffLabel->setString(txt.c_str());
    m_diffLabel->setColor(col);
    
    if (mgr->isCompleted(level->id)) {
        m_progressLabel->setString("Completed!");
        m_progressLabel->setColor({0, 255, 0});
    } else {
        int p = mgr->getBestProgress(level->id);
        m_progressLabel->setString(fmt::format("Progress: {}%", p).c_str());
        m_progressLabel->setColor({255, 255, 255});
    }
}

void RandomRouletteLayer::onPlay(CCObject*) {
    auto level = RandomRouletteManager::getInstance()->getCurrentLevel();
    if (!level) return;
    
    // Open level browser directly with search by ID
    auto searchObj = GJSearchObject::create(SearchType::Type19, std::to_string(level->id));
    auto browser = LevelBrowserLayer::scene(searchObj);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, browser));
    
    this->onClose(nullptr);
}

void RandomRouletteLayer::onNext(CCObject*) {
    int id = RandomRouletteManager::getInstance()->pickRandomLevel();
    if (id == -1) {
        FLAlertLayer::create("Info", "All demons completed!", "OK")->show();
    }
    updateUI();
}

void RandomRouletteLayer::onReset(CCObject*) {
    createQuickPopup("Reset", "Are you sure?", "Cancel", "Reset", [this](auto, bool res){
        if (res) {
            RandomRouletteManager::getInstance()->resetRoulette();
            updateUI();
        }
    });
}
