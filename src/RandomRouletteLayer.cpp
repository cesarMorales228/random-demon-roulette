#include "RandomRouletteLayer.hpp"

const std::vector<RecommendedLevel>& getRecommendedLevels() {
    static std::vector<RecommendedLevel> levels = {
        {10565740, "Bloodbath", "Riot", 10},
        {42584142, "Tartarus", "Riot", 10},
        {61079355, "Acu", "neigefeu", 10},
        {3979721, "Cataclysm", "Ggb0y", 10},
        {77151846, "Slaughterhouse", "Icedcave", 10},
        {56165243, "Zodiac", "Xander556", 10},
        {61765103, "The Golden", "BoBoBoBoBoBo", 10},
        {10651760, "Sonic Wave", "Cyclic", 10},
        {39669528, "Kenos", "Rustam", 10},
        {81014878, "Acheron", "Ryamu", 10},
        {8660411, "Platinum Adventure", "Jerry", 6},
        {122607, "Demon Mixed", "RealOggY", 4},
        {3307584, "The Lightning Road", "Timeless", 5},
        {3633830, "The Nightmare", "Jax", 4},
        {7828189, "Speed Racer", "ZenthicAlpha", 5},
        {97486, "Theory of Everything 2", "RobTop", 3},
        {191199, "RadioActive", "MrCheeseTigrr", 4},
        {422209, "Impulse", "MrCheeseTigrr", 4},
        {192994, "Crescendo", "MasK463", 5},
        {1111, "xStep v2", "Neptune", 4}
    };
    return levels;
}

RandomRouletteLayer* RandomRouletteLayer::create() {
    auto ret = new RandomRouletteLayer();
    if (ret && ret->initAnchored(420.f, 280.f)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool RandomRouletteLayer::setup() {
    this->setTitle("Level Recommender");
    
    // Create scroll layer
    auto scrollLayer = ScrollLayer::create({400.f, 200.f});
    scrollLayer->setPosition(m_size.width / 2 - 200.f, 40.f);
    m_mainLayer->addChild(scrollLayer);
    
    // Create cells for each level
    float yOffset = 0;
    auto& levels = getRecommendedLevels();
    
    auto contentMenu = CCMenu::create();
    contentMenu->setPosition(0, 0);
    
    for (const auto& level : levels) {
        createLevelCell(level, yOffset, contentMenu);
        yOffset -= 30.f;
    }
    
    scrollLayer->m_contentLayer->setContentSize({400.f, std::abs(yOffset) + 30.f});
    scrollLayer->m_contentLayer->addChild(contentMenu);
    scrollLayer->moveToTop();
    
    return true;
}

void RandomRouletteLayer::createLevelCell(RecommendedLevel level, float yPos, CCMenu* menu) {
    // Background
    auto bg = CCLayerColor::create({0, 0, 0, 100}, 390.f, 25.f);
    bg->setPosition(5, yPos);
    menu->addChild(bg, -1);
    
    // Name label
    auto nameLabel = CCLabelBMFont::create(level.name.c_str(), "bigFont.fnt");
    nameLabel->setScale(0.4f);
    nameLabel->setAnchorPoint({0, 0.5f});
    nameLabel->setPosition(15, yPos + 12.5f);
    menu->addChild(nameLabel, 1);
    
    // Creator label
    auto creatorLabel = CCLabelBMFont::create(fmt::format("by {}", level.creator).c_str(), "goldFont.fnt");
    creatorLabel->setScale(0.3f);
    creatorLabel->setAnchorPoint({0, 0.5f});
    creatorLabel->setPosition(200, yPos + 12.5f);
    menu->addChild(creatorLabel, 1);
    
    // Play button
    auto playSpr = ButtonSprite::create("Play", "goldFont.fnt", "GJ_button_01.png", 0.6f);
    auto playBtn = CCMenuItemSpriteExtra::create(
        playSpr,
        this,
        menu_selector(RandomRouletteLayer::onPlayLevel)
    );
    playBtn->setTag(level.id);
    playBtn->setPosition(350, yPos + 12.5f);
    menu->addChild(playBtn);
}

void RandomRouletteLayer::onPlayLevel(CCObject* sender) {
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    int levelID = btn->getTag();
    
    auto searchObj = GJSearchObject::create(SearchType::Type19, std::to_string(levelID));
    auto browser = LevelBrowserLayer::scene(searchObj);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, browser));
    
    this->onClose(nullptr);
}
