#include "Weapon.h"
#include "Locator.h"
#include <cstdint>
#include <cmath>
#include <iostream>

using namespace std;


const int Weapon::ACCURACY_DIVISOR = 16129;

//Handles firing at a target
shotResult Weapon::fire(HealthTracker& target) {

    //if weapon needs setting up, abort firing
    if ((isSetupToFireWeapon) && (setupStatus == false)) return shotResult(shotResultFlag::DID_NOT_FIRE);

    //if weapon uses ammunition and is out, abort firing
    if (usesAmmo) {
        if (hasMagazine) {
            if (needsReload) {
                if (DEBUG_MSG1) cout << "DEBUG: Weapon::fire code 0010: returning needs reload\n";
                return shotResult(shotResultFlag::DID_NOT_FIRE);
            }
        }
        else if (outOfAmmo) {
            if (DEBUG_MSG1) cout << "DEBUG: Weapon::fire code 0015: returning out of ammo\n";
            return shotResult(shotResultFlag::DID_NOT_FIRE);
        }
    }

    //Calculate range and accuracy
    float rangeToTarget = target.location.getLocation() - location.getLocation();

    //Calculate accuracy based on range;
    int tempAccuracy;
    if (rangeToTarget > maxSightingRange)
        tempAccuracy = 0;
    else if (rangeToTarget > maxRange)
        tempAccuracy = 0;
    else if (rangeToTarget > maxOptimalRange)
        tempAccuracy = (int)(((maxSightingRange - rangeToTarget) * baseAccuracy) / (maxSightingRange - maxOptimalRange));
    else if (rangeToTarget > closeRange)
        tempAccuracy = baseAccuracy;
    else
        tempAccuracy = closeRangeAccuracy;
    if (tempAccuracy == 0) { //abort firing if impossible shot
        if (DEBUG_MSG1) cout << "DEBUG: Weapon::fire code 0020: returning zero accuracy based on range\n";
        return shotResult(shotResultFlag::DID_NOT_FIRE);
    }
    if (DEBUG_MSG1) cout << "DEBUG: Weapon::fire code 0025: tempAccuracy = " << tempAccuracy << endl;

    //Adjust accuracy due to firing angle
    int8_t fireAngle = angleOfElevation(location.getLocation(), target.location.getLocation());
    if (fireAngle > maximumFireAngle)
        tempAccuracy = 0;
    else if (fireAngle < minimumFireAngle)
        tempAccuracy = 0;
    else if (fireAngle >= optimalFireAngle)
        tempAccuracy += (fireAngleDependenceHigh * (fireAngle - optimalFireAngle) / 32);
    else
        tempAccuracy += (fireAngleDependenceLow * (optimalFireAngle - fireAngle) / 32);
    if (tempAccuracy == 0) { //abort firing if impossible shot
        if (DEBUG_MSG1) cout << "DEBUG: Weapon::fire code 0030: returning zero accuracy based on fire angle\n";
        return shotResult(shotResultFlag::DID_NOT_FIRE);
    }
    if (DEBUG_MSG1) cout << "DEBUG: Weapon::fire code 0040: tempAccuracy = " << tempAccuracy << endl;
    //currently tempAccuracy is on a scale from 0-255

    //Adjust accuracy for stability
    if (stability < halfStability)
        tempAccuracy = 0;
    else if (stability < fullStability)
        tempAccuracy *= halfStabilityPenaltyMult; //63 = 50%, 127 = 100%, 255 = 200%
    else
        tempAccuracy *= 127;
    if (tempAccuracy == 0) { //abort firing if impossible shot
        if (DEBUG_MSG1) cout << "DEBUG: Weapon::fire code 0050: returning zero accuracy based on stability\n";
        return shotResult(shotResultFlag::DID_NOT_FIRE);
    }
    if (DEBUG_MSG1) cout << "DEBUG: Weapon::fire code 0060: tempAccuracy = " << tempAccuracy << endl;
    //now tempAccuracy is on a scale from 0-32385 for 0-100%, but can go up to 65025 (200%)

    //Adjust accuracy for target size; recall definitions from class:
        //uint8_t smallTargetAccuracyMult;    //127 = 100%, 255 = 200%
        //uint8_t largeTargetAccuracyMult;    //127 = 100%, 255 = 200%
    if (target.getTargetSize() <= smallTargetSize) tempAccuracy *= smallTargetAccuracyMult;
    else if (target.getTargetSize() >= largeTargetSize) tempAccuracy *= largeTargetAccuracyMult;
    else tempAccuracy *= 255;
    if (DEBUG_MSG1) cout << "DEBUG: Weapon::fire code 0070: tempAccuracy = " << tempAccuracy << endl;
    //now tempAccuracy is on a scale from 0-4112895 for 0-100%, but can go up to 16581375 (400%)

    //Final calculation of accuracy
    tempAccuracy = tempAccuracy / ACCURACY_DIVISOR; //crunch down to 0-255 range
    if (tempAccuracy > 255) tempAccuracy = 255;     //if >100%, =100% accuracy
    if (DEBUG_MSG1) cout << "DEBUG: Weapon::fire code 0080: final tempAccuracy = " << tempAccuracy << endl;

    //Build shot to pass to target
    shot_T shot;
    shot.accuracy = (uint8_t) tempAccuracy;

    //Calculate critical hit info
    shot.critChance = critChance;
    shot.critMult = critMult;

    //Connect weapons damage and piercing coefficients to shot
    shot.damage = damage;
    shot.piercingCoeffs = piercingCoeffs;

    //pass the shot to the target's takeFire function
    shotResult result = target.takeFire(shot);

    //If the weapon uses ammunition, decrement
    if (usesAmmo) {
        if (hasMagazine) {
            ammoInMagazine--;
            if (ammoInMagazine == 0) needsReload = true;
        }
        else {
            ammo--;
            if (ammo == 0) outOfAmmo = true;
        }
    }

    //TODO: I'll need another section for if the weapon requires electric energy per shot

    //TODO: I'll need to implement noise generation

    return result;

}














// Debug functions



void Weapon::debug_setAccuracy(uint8_t baseAccuracy_in, uint8_t closeRangeAccuracy_in) {
    if (DEBUG_ENABLED) {
        baseAccuracy = baseAccuracy_in;
        closeRangeAccuracy = closeRangeAccuracy_in;
    }
}

void Weapon::debug_setRange(short maxOptimalRange_in, short closeRange_in, short maxSightingRange_in, short maxRange_in) {
    if (DEBUG_ENABLED) {
        maxOptimalRange = maxOptimalRange_in;
        closeRange = closeRange_in;
        maxSightingRange = maxSightingRange_in;
        maxRange = maxRange_in;
    }
}

void Weapon::debug_setTargetingSizeParams(uint8_t smallTargetSize_in, uint8_t largeTargetSize_in, uint8_t smallTargetAccuracyMult_in, uint8_t largeTargetAccuracyMult_in){
    if (DEBUG_ENABLED) {
        smallTargetSize = smallTargetSize_in;
        largeTargetSize = largeTargetSize_in;
        smallTargetAccuracyMult = smallTargetAccuracyMult_in;
        largeTargetAccuracyMult = largeTargetAccuracyMult_in;
    }
}

void Weapon::debug_setStabilityParams(uint8_t halfStability_in, uint8_t fullStability_in, uint8_t halfStabilityPenaltyMult_in) {
    if (DEBUG_ENABLED) {
        halfStability = halfStability_in;
        fullStability = fullStability_in;
        halfStabilityPenaltyMult = halfStabilityPenaltyMult_in;
    }
}

void Weapon::debug_setFireAngleParams(int8_t minimumFireAngle_in, int8_t maximumFireAngle_in,
                              int8_t optimalFireAngle_in, int8_t fireAngleDependenceHigh_in,
                              int8_t fireAngleDependenceLow_in) {
    if (DEBUG_ENABLED) {
        minimumFireAngle = minimumFireAngle_in;
        maximumFireAngle = maximumFireAngle_in;
        optimalFireAngle = optimalFireAngle_in;
        fireAngleDependenceHigh = fireAngleDependenceHigh_in;
        fireAngleDependenceLow = fireAngleDependenceLow_in;
    }
}

void Weapon::debug_setAmmoParams(bool usesAmmo_in, short ammo_in, bool hasMagazine_in,
                         short ammoInMagazine_in, short timeToReload_in,
                         unsigned int magazineCapacity_in) {
    if (DEBUG_ENABLED) {
        usesAmmo = usesAmmo_in;
        ammo = ammo_in;
        hasMagazine = hasMagazine_in;
        ammoInMagazine = ammoInMagazine_in;
        timeToReload = timeToReload_in;
        magazineCapacity = magazineCapacity_in;
        if (hasMagazine) {
            if (ammoInMagazine_in > 0) {
                needsReload = false;
                outOfAmmo = false;
            }
            else {
                needsReload = true;
                if (ammo > 0) outOfAmmo = false;
                else outOfAmmo = true;
            }
        }
        else {
            if (ammo > 0) {
                outOfAmmo = false;
            }
            else {
                outOfAmmo = true;
            }
        }
    }
}

void Weapon::debug_setCritParams(uint8_t critChance_in, float critMult_in) {
    if (DEBUG_ENABLED) {
        critChance = critChance_in;
        critMult = critMult_in;
    }
}

void Weapon::debug_setSetupToFire(bool isSetupToFireWeapon_in, bool setupStatus_in) {
    if (DEBUG_ENABLED) {
        isSetupToFireWeapon = isSetupToFireWeapon_in;
        setupStatus = setupStatus_in;
    }
}

void Weapon::debug_setDamage(short damage_in[]) {
    if (DEBUG_ENABLED) {
        for (int i = 0; i < NUM_DAMAGE_TYPES; i++)
            damage[i] = damage_in[i];
    }
}


