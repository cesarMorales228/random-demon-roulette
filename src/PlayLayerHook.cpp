#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "RandomRouletteManager.hpp"
#include <Geode/loader/Event.hpp>

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
    void levelComplete() {
        PlayLayer::levelComplete();
        
        auto mgr = RandomRouletteManager::sharedState();
        if (m_level->m_levelID == mgr->getCurrentLevelID() && m_level->m_levelID > 0) {
            mgr->markLevelCompleted(m_level->m_levelID);
            Notification::create("Nivel de Ruleta Completado!", NotificationIcon::Success)->show();
        }
    }

    void destroyPlayer(PlayerObject* p0, GameObject* p1) {
        PlayLayer::destroyPlayer(p0, p1);

        // Check if it's the main player dying
        if (!m_isPracticeMode && p0 == m_player1) {
           auto mgr = RandomRouletteManager::sharedState();
           if (m_level->m_levelID == mgr->getCurrentLevelID() && m_level->m_levelID > 0) {
               // Get current percentage safely
               // m_level->m_normalPercent is updated continuously
               mgr->updateProgress(m_level->m_levelID, m_level->m_normalPercent);
           }
        }
    }
    
    // Also catch quit to menu to save progress
    void onQuit() {
        PlayLayer::onQuit();
        
        if (!m_isPracticeMode) {
             auto mgr = RandomRouletteManager::sharedState();
             // Ensure level pointer is valid before using
             if (m_level && m_level->m_levelID == mgr->getCurrentLevelID() && m_level->m_levelID > 0) {
                 mgr->updateProgress(m_level->m_levelID, m_level->m_normalPercent);
             }
        }
    }
};
