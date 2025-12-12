#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "RandomRouletteManager.hpp"
#include <Geode/loader/Event.hpp>

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
    void levelComplete() {
        PlayLayer::levelComplete();
        
        auto mgr = RandomRouletteManager::getInstance();
        auto current = mgr->getCurrentLevel();
        if (current && m_level->m_levelID == current->id && m_level->m_levelID > 0) {
            mgr->markCompleted(m_level->m_levelID);
            Notification::create("Roulette Level Verified!", NotificationIcon::Success)->show();
        }
    }

    void destroyPlayer(PlayerObject* p0, GameObject* p1) {
        PlayLayer::destroyPlayer(p0, p1);

        if (!m_isPracticeMode && p0 == m_player1) {
           auto mgr = RandomRouletteManager::getInstance();
           auto current = mgr->getCurrentLevel();
           if (current && m_level && m_level->m_levelID == current->id && m_level->m_levelID > 0) {
               // Safe percent capture
               mgr->updateProgress(m_level->m_levelID, m_level->m_normalPercent);
           }
        }
    }
};
