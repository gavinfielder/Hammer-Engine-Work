#ifndef WEAPON_H
#define WEAPON_H

#include "Global.h"
#include "HealthTracker.h"
#include <cstdint>


//Class Weapon is an abstract base class for objects that can damage HealthTracker objects
class Weapon {
    public:
        //Constructors
        Weapon() {}

        //Functionality
        shotResult fire(HealthTracker& target); //TODOL I'll need to change this to accept GameObject instead

        //Location
        Locator location;

        //Debug functions
        void debug_setDamage(short damage_in[]);
        void debug_setAccuracy(uint8_t baseAccuracy_in, uint8_t closeRangeAccuracy_in);
        void debug_setRange(short maxOptimalRange_in, short closeRange_in, short maxSightingRange_in, short maxRange_in);
        void debug_setTargetingSizeParams(uint8_t smallTargetSize_in, uint8_t largeTargetSize_in, uint8_t smallTargetAccuracyMult_in, uint8_t largeTargetAccuracyMult_in);
        void debug_setStabilityParams(uint8_t halfStability_in, uint8_t fullStability_in, uint8_t halfStabilityPenaltyMult_in);
        void debug_setStability(uint8_t stability_in)
            { if (DEBUG_ENABLED) stability = stability_in; }
        void debug_setFireAngleParams
            (int8_t minimumFireAngle_in, int8_t maximumFireAngle_in, int8_t optimalFireAngle_in, int8_t fireAngleDependenceHigh_in, int8_t fireAngleDependenceLow_in);
        void debug_setAmmoParams
            (bool usesAmmo_in, short ammo_in, bool hasMagazine_in, short ammoInMagazine_in, short timeToReload_in, unsigned int magazineCapacity_in);
        void debug_setCritParams(uint8_t critChance_in, float critMult_in);
        void debug_setSetupToFire(bool isSetupToFireWeapon_in, bool setupStatus_in);

    protected:
        //Basic stats
        short damage[NUM_DAMAGE_TYPES];
        unsigned short timeBetweenShots;         //replaces ROF

        //Accuracy
        uint8_t baseAccuracy;
        //Accuracy due to range
        short closeRange;            //threshold for close range
        short maxOptimalRange;       //ranges above this have reduced accuracy
        short maxRange;              //ranges above this cannot be hit
        short maxSightingRange;      //maximum range that targets can be sighted
        uint8_t closeRangeAccuracy;  //accuracy below the close range threshold

        //Accuracy due to target size
        uint8_t smallTargetSize;     //targets below this size have reduced accuracy
        uint8_t largeTargetSize;     //targets above this size have increased accuracy
        uint8_t smallTargetAccuracyMult;    //127 = 100%, 255 = 200%
        uint8_t largeTargetAccuracyMult;    //127 = 100%, 255 = 200%

        //Stability - determines platform capability, mobility, etc - affects accuracy
        uint8_t halfStability;              //minimum stability required to fire
        uint8_t fullStability;              //stability required for no penalty to accuracy
        uint8_t halfStabilityPenaltyMult;   //63 = 50%, 127 = 100%, 255 = 200%
        uint8_t stability;                  //current stability
        //TODO: I need a time variable here to track time since last stop. Data type?

        //Firing angle
        int8_t minimumFireAngle;              //lowest possible fire angle   (-128 = -90 deg,
        int8_t maximumFireAngle;              //highest possible fire angle    127 = 90 deg  )
        int8_t optimalFireAngle;              //optimal fire angle for full accuracy
        //Firing angle accuracy parameters - affects accuracy
        int8_t fireAngleDependenceHigh;  //each angle above optimum adds accuracy by 1/32 of this
        int8_t fireAngleDependenceLow;   //each angle below optimum adds accuracy by 1/32 of this
        //example: setting fireAngleDependenceHigh = -24 means that
        //         an angle of 35 (~25 deg up) reduces accuracy by about 26 (~10%)


        //Accuracy calculation divisor
        const static int ACCURACY_DIVISOR;

        //Ammunition and magazine
        bool usesAmmo;
        short ammo;    //if uses magazines, this is reserve ammo
        bool hasMagazine;
        short ammoInMagazine;
        bool needsReload;
        bool outOfAmmo;
        short timeToReload;
        unsigned int magazineCapacity;

        //Critical hits
        uint8_t critChance;
        float critMult;

        //Setup-to-fire weapons
        bool isSetupToFireWeapon;
        bool setupStatus;

        //Piercing capability
        uint8_t piercingCoeffs[NUM_DAMAGE_TYPES];

        //AOE weapons
        //TODO: I have to work this section carefully. Probably need an "explosion" class

        //Power
        bool requiresPower;
        energy_T kiloWattDrain;
        energy_T kiloWattHourPerShot;

        //Other
        uint8_t noiseLevel;

        //Special status effects
        //TODO: I have to work this section carefully.


};

#endif // WEAPON_H
