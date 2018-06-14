#include "HealthTracker.h"
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <iostream>

#define DAMAGE_DIVISOR 127   //should allow 0 damage up to double damage



//Default constructor
HealthTracker::HealthTracker() {
    hitPoints = 1;
    //resist = nullptr;
    targetSize = 0;
    destroyed = false;
    criticalHitResistance = 0;
}


//This handles the damageable object taking fire from a weapon
//returns true if hit, false if miss
//TODO: implement piercing coefficients
shotResult HealthTracker::takeFire(shot_T shot) {
    shotResult result;
    if ((rand() % 256) < shot.accuracy) {
        //hit; calculate damage
        float mult;
        float damage = 0;
        for (int i = 0; i < NUM_DAMAGE_TYPES; i++) {
            mult = ((float)(255 - resist[i])) / DAMAGE_DIVISOR;
            damage += mult * shot.damage[i];
        }
        //determine if critical hit
        if ((rand() % 256) < (shot.critChance - criticalHitResistance)) {
            result.success = shotResultFlag::CRITICAL_HIT;
            damage *= shot.critMult;
        }
        else result.success = shotResultFlag::HIT;
        //apply damage
        if (hitPoints - ((int)damage) <= 0) {
            hitPoints = 0;
            destroyed = true;
            result.destroyed = true;
        }
        else {
            hitPoints -= (short)damage;
        }
        //return damage
        result.damage = damage;
    }
    else {
        //miss; return 0 damage
        //default initialization is miss and zero damage, so nothing to do
    }
    return result;
}





//Debug functions

void HealthTracker::debug_setResist(uint8_t resist_in[]) {
    if (DEBUG_ENABLED) {
        for (int i = 0; i < NUM_DAMAGE_TYPES; i++)
            resist[i] = resist_in[i];
    }
}

















