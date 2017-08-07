//
// Created by monty on 22/10/16.
//
#include <functional>
#include "Vec2i.h"
#include "CGameDelegate.h"

namespace Knights {

    CGameDelegate::CGameDelegate() {
        mOnMonsterDead = [](Knights::Vec2i pos) { };
        mOnPlayerDead = [](Knights::Vec2i pos) { };
        mOnPlayerAttack = [](Knights::Vec2i pos) { };
        mOnMonsterAttack = [](Knights::Vec2i pos) { };
        mOnMonsterDamaged = [](Knights::Vec2i pos) { };
        mOnPlayerDamaged = [](Knights::Vec2i pos) { };
        mOnProjectileHit = [](Knights::Vec2i pos) { };
        mOnLevelLoaded = []() { };
    }

    void CGameDelegate::setMonsterDiedCallback(std::function<void(Knights::Vec2i)> aCallback) {
        mOnMonsterDead = aCallback;
    }

    void CGameDelegate::setPlayerDiedCallback(std::function<void(Knights::Vec2i)> aCallback) {
        mOnPlayerDead = aCallback;
    }

    void CGameDelegate::setMonsterAttackedCallback(std::function<void(Knights::Vec2i)> aCallback) {
        mOnMonsterAttack = aCallback;
    }

    void CGameDelegate::setPlayerAttackedCallback(std::function<void(Knights::Vec2i)> aCallback) {
        mOnPlayerAttack = aCallback;
    }

    void CGameDelegate::setPlayerDamagedCallback(std::function<void(Knights::Vec2i)> aCallback) {
        mOnPlayerDamaged = aCallback;
    }

    void CGameDelegate::setMonsterDamagedCallback(std::function<void(Knights::Vec2i)> aCallback) {
        mOnMonsterDamaged = aCallback;
    }

    void CGameDelegate::setOnLevelLoadedCallback(std::function<void(void)> aCallback) {
        mOnLevelLoaded = aCallback;
    }

    void CGameDelegate::setProjectileCallback(std::function<void(Knights::Vec2i)> aCallback) {
        mOnProjectileHit = aCallback;
    }

    void CGameDelegate::onPlayerAttacked(Knights::Vec2i pos) {
        mOnPlayerAttack(pos);
    }

    void CGameDelegate::onMonsterAttacked(Knights::Vec2i pos) {
        mOnMonsterAttack(pos);
    }

    void CGameDelegate::onPlayerDied(Knights::Vec2i pos) {
        mOnPlayerDead(pos);
    }

    void CGameDelegate::onMonsterDied(Knights::Vec2i pos) {
        mOnMonsterDead(pos);
    }

    void CGameDelegate::onMonsterDamaged(Knights::Vec2i pos) {
        mOnMonsterDamaged(pos);
    }

    void CGameDelegate::onPlayerDamaged(Knights::Vec2i pos) {
        mOnPlayerDamaged(pos);
    }

    void CGameDelegate::onProjectileHit(Knights::Vec2i pos) {
        mOnProjectileHit(pos);
    }

    void CGameDelegate::onLevelLoaded() {
        mOnLevelLoaded();
    }
}
