#ifndef HEALTHTRACKER_H
#define HEALTHTRACKER_H

#include "Global.h"
#include "Locator.h"
#include <cstdint>

//(comment for my educational benefit) Strongly typed enum with underlying type as int8_t (1 byte!)
enum class shotResultFlag : int8_t {
    DID_NOT_FIRE = -1,
    MISS = 0,
    HIT = 1,
    CRITICAL_HIT = 2
};

//Class shotResult holds information about a shot to return to the shooter
class shotResult {
    public:
        //Information
        shotResultFlag success;
        unsigned short damage;
        bool destroyed;
        //Constructors
        shotResult() { destroyed = false; success = shotResultFlag::MISS; damage = 0; }
        shotResult(shotResultFlag success_in, unsigned short damage_in, bool destroyed_in) {
            success = success_in;
            damage = damage_in;
            destroyed = destroyed_in;
        }
        shotResult(shotResultFlag success_in) {
            success = success_in;
            damage = 0;
            destroyed = false;
        }
};


//Version 0606
//Class HealthTracker is used for objects that can take damage and be destroyed
class HealthTracker {
    public:
        //Constructors
        HealthTracker();

        //Functionality
        shotResult takeFire(shot_T shot); //might need to make this virtual in the future

        //Location
        Locator location; //I don't want to implement this here. TODO: move

        //Accessors
        short getHP() { return hitPoints; }
        uint8_t getResist(int index) { return resist[index]; }
        uint8_t getTargetSize() { return targetSize; }
        bool isDestroyed() { return destroyed; }

        //Debug functions
        void debug_setHitPoints(short hitPoints_in) { if (DEBUG_ENABLED) hitPoints = hitPoints_in; }
        void debug_setResist(uint8_t resist_in[]);
        void debug_setTargetSize(uint8_t targetSize_in) { if (DEBUG_ENABLED) targetSize = targetSize_in; }
        void debug_setCriticalHitResistance(int8_t criticalHitResistance_in) { if (DEBUG_ENABLED) criticalHitResistance = criticalHitResistance_in; }

    protected:
        short hitPoints;
        uint8_t resist[NUM_DAMAGE_TYPES];
        uint8_t targetSize;
        bool destroyed;
        int8_t criticalHitResistance;

};

//The Null HealthTracker -- this is returned if a HealthTracker was requested but none is available
const HealthTracker NULL_HEALTHTRACKER;





/*
//Version 0605 - deprecated due to moving Damageable (MI) to HealthTracker (composition)
//Class damageable is an abstract base class for objects that can take damage and be destroyed
class Damageable {
    public:
        //Constructors
        Damageable() {}

        //Functionality
        shotResult takeFire(shot_T shot); //might need to make this virtual in the future

        //Location
        Locator location;

        //Accessors
        short getHP() { return hitPoints; }
        uint8_t getResist(int index) { return resist[index]; }
        uint8_t getTargetSize() { return targetSize; }
        bool isDestroyed() { return destroyed; }

        //Debug functions
        void debug_setHitPoints(short hitPoints_in) { if (DEBUG_ENABLED) hitPoints = hitPoints_in; }
        void debug_setResist(uint8_t resist_in[]);
        void debug_setTargetSize(uint8_t targetSize_in) { if (DEBUG_ENABLED) targetSize = targetSize_in; }
        void debug_setCriticalHitResistance(int8_t criticalHitResistance_in) { if (DEBUG_ENABLED) criticalHitResistance = criticalHitResistance_in; }

    protected:
        short hitPoints;
        uint8_t resist[NUM_DAMAGE_TYPES];
        uint8_t targetSize;
        bool destroyed;
        int8_t criticalHitResistance;

};
*/


/*
//Version 0603: deprecated by moving Locateable (MI) to Locator (composition)
//Class damageable is an abstract base class for objects that can take damage and be destroyed
class Damageable : public Locateable {
    public:
        //Constructors
        Damageable() {}

        //Functionality
        shotResult takeFire(shot_T shot); //might need to make this virtual in the future

        //Accessors
        short getHP() { return hitPoints; }
        uint8_t getResist(int index) { return resist[index]; }
        uint8_t getTargetSize() { return targetSize; }
        bool isDestroyed() { return destroyed; }

        //Debug functions
        void debug_setHitPoints(short hitPoints_in) { if (DEBUG_ENABLED) hitPoints = hitPoints_in; }
        void debug_setResist(uint8_t resist_in[]);
        void debug_setTargetSize(uint8_t targetSize_in) { if (DEBUG_ENABLED) targetSize = targetSize_in; }
        void debug_setCriticalHitResistance(int8_t criticalHitResistance_in) { if (DEBUG_ENABLED) criticalHitResistance = criticalHitResistance_in; }

    protected:
        short hitPoints;
        uint8_t resist[NUM_DAMAGE_TYPES];
        uint8_t targetSize;
        bool destroyed;
        int8_t criticalHitResistance;

};
*/


#endif // DAMAGEABLE_H
