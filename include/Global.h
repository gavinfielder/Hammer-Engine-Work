#ifndef GLOBAL_H
#define GLOBAL_H


/*
 * This file holds constants and types used throughout the classes
 *
 *
 *
 */


/*
 * General information:
 *      Use short for time values, damage, and hitPoints
 *
 */

 /*
 * TODO:
 *
 *    Transition to smart pointers:
 *          Locator
 *    change NULL back to nullptr
 *
 */


#include <cstdlib>
#include <ctime>
#include <cstdint>

//using namespace std;

//General use functions
void seedRandomNumberGenerator();

//Global constants
#define NUM_DAMAGE_TYPES 16
#define PI 3.14159265
const bool DEBUG_ENABLED = true;
const bool DEBUG_MSG1 = false;

//Global types

//Handles a shot from a weapon at a target
typedef struct shot_struct {
    short* damage; //short damage[NUM_DAMAGE_TYPES];
    uint8_t* piercingCoeffs; //short piercingCoeffs[NUM_DAMAGE_TYPES];
    float critMult;
    uint8_t critChance;
    uint8_t accuracy;
} shot_T;

//Handles a value for a unit of power
typedef struct energy_struct {
    short value;
    int8_t order;
} energy_T;
//Returns an energy based on passed parameters
energy_T newEnergy(short value, char order);


#endif // GLOBAL_H
