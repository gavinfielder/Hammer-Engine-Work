
/*
 * This file holds constants and types used throughout the classes
 *
 *
 *
 */
#ifndef GLOBALCONSTANTS_H
#define GLOBALCONSTANTS_H


/*
 * General information:
 *      Use short for time values, damage, and hitpoints
 *
 */


#include <cstdlib>
#include <ctime>
#include <cstdint>

using namespace std;

//General use functions

void seedRandomNumberGenerator() {
    srand(time(NULL));
}

//Global constants
#define NUM_DAMAGE_TYPES 16
#define DAMAGE_DIVISOR 127   //should allow 0 damage up to double damage

//Global types

//Handles a shot from a weapon at a target
typedef struct shot_struct {
    short damage[NUM_DAMAGE_TYPES];
    float critMult;
    uint8_t critChance;
    int8_t accuracy;
} shot_T;

//Handles a value for a unit of power
typedef struct energy_struct {
    short value;
    int8_t order;
} energy_T;
//Returns an energy based on passed parameters
energy_T newEnergy(short value, char order) {
    energy_T toReturn;
    toReturn.value = value;
    toReturn.order = order;
    return toReturn;
}


#endif // GLOBALCONSTANTS_H
