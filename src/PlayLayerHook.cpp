#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "RandomRouletteManager.hpp"

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
    void levelComplete() {
        PlayLayer::levelComplete();
        
        auto mgr = RandomRouletteManager::sharedState();
        if (m_level->m_levelID == mgr->getCurrentLevelID() && m_level->m_levelID > 0) {
            mgr->markLevelCompleted(m_level->m_levelID);
            Notification::create("Roulette Demon Completed!", NotificationIcon::Success)->show();
        }
    }

    void destroyPlayer(PlayerObject* p0, GameObject* p1) {
        PlayLayer::destroyPlayer(p0, p1);

        if (!m_isPracticeMode && p0 == m_player1) {
           auto mgr = RandomRouletteManager::sharedState();
           // Strict null checking for m_level
           if (m_level && m_level->m_levelID == mgr->getCurrentLevelID() && m_level->m_levelID > 0) {
               mgr->updateProgress(m_level->m_levelID, m_level->m_normalPercent);
           }
        }
    }

    void onQuit() {
        PlayLayer::onQuit();
        
        if (!m_isPracticeMode) {
             auto mgr = RandomRouletteManager::sharedState();
             if (m_level && m_level->m_levelID == mgr->getCurrentLevelID() && m_level->m_levelID > 0) {
                 mgr->updateProgress(m_level->m_levelID, m_level->m_normalPercent);
             }
        }
    }
};
