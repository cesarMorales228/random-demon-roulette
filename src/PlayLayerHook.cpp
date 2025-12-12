#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "RandomRouletteManager.hpp"

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
    void levelComplete() {
        PlayLayer::levelComplete();
        
        // Check if we are playing the roulette level
        auto mgr = RandomRouletteManager::sharedState();
        int rouletteID = mgr->getCurrentLevelID();
        
        if (m_level->m_levelID == rouletteID && rouletteID > 0) {
            mgr->updateProgress(m_level->m_levelID, 100);
            mgr->markLevelCompleted(m_level->m_levelID);
            
            Notification::create("Roulette Level Completed!", NotificationIcon::Success)->show();
        }
    }

    // Capture specific percentage updates if needed, e.g. on death
    // Hooks destroy() to catch when leaving the layer or dying and restarting
    // However, capturing "best run" is tricky on destroy because m_level might be resetting.
    // Better to hook `playerDestroyed`.
    
    void playerDestroyed(bool p0) {
        PlayLayer::playerDestroyed(p0);
        
        auto mgr = RandomRouletteManager::sharedState();
        int rouletteID = mgr->getCurrentLevelID();
        
        if (m_level->m_levelID == rouletteID && rouletteID > 0) {
            // Get current percentage logic
             // m_level->m_normalPercent should hold current run percent if we are carefully reading it. 
             // Actually PlayLayer calculates it based on X pos. 
             // Simpler approach: check `getCurrentPercent()` if available or `m_level->m_normalPercent`.
             // Note: getCurrentPercent is helper.
             
             // We'll trust m_level->m_normalPercent update which usually happens in update()
             int p = getCurrentPercentInt();
             mgr->updateProgress(rouletteID, p);
        }
    }
};
