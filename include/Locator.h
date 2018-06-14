#ifndef LOCATOR_H
#define LOCATOR_H

#include <cmath>
#include <cstdint>
#include <iostream>
#include "Global.h"


typedef struct point3f_struct {
    float x;
    float y;
    float z;
} point3f_T;

//The - operator returns the distance between two locations
float operator-(point3f_T a, point3f_T b);

//The + operator returns the sum of two point3f's
point3f_T operator+(point3f_T a, point3f_T b);

//Returns the angle of elevation from observer to target points, represented as follows:
//     0 is horizontal
//     +/- 127 is vertical
int8_t angleOfElevation(point3f_T observer, point3f_T target);



//Version 0606: see below for older versions
//Class Locator is used in objects that have a location
class Locator {
    public:
        Locator();
        ~Locator();

        //Accessors
        point3f_T getLocation();
        uint8_t getNumberOfChildren() { return numberOfChildren; }
        Locator& getChild(uint8_t index) { return *(children[index]); }

        //Manipulators
        void setLocation(point3f_T newLocation);
        void moveLocation(point3f_T change);

        //Functions for creating a tree so objects can share locations
        void linkChildLocator(Locator& child);
        void unlinkChildLocator(Locator& child);
        void unlinkAllChildrenLocators();

        //Debug functions

    private:
        point3f_T* location;        //I think this one's fine as a normal pointer -- not intended to be allocated all the time, etc
        Locator* parent;         //TODO: transition to std::shared_ptr
        Locator** children;      //TODO: transition to std::shared_ptr * (array of std::shared_ptr 's)
        uint8_t numberOfChildren;       //or maybe the above should be std::weak_ptr ?
};


//The null Locator -- this is returned if a Locator was requested but none is available
const Locator NULL_LOCATOR;



/*
//Version 0603B: deprecated by moving to composition rather than planning multiple inheritance
//Class locateable is an abstract base class for objects that have a location
class Locateable {
    public:
        Locateable();
        ~Locateable();

        //Accessors
        point3f_T getLocation();
        uint8_t getNumberOfChildren() { return numberOfChildren; }
        Locateable& getChild(uint8_t index) { return *(children[index]); }

        //Manipulators
        void setLocation(point3f_T newLocation);
        void moveLocation(point3f_T change);

        //Functions for creating a tree so objects can share locations
        void linkChildLocateable(Locateable& child);
        void unlinkChildLocateable(Locateable& child);
        void unlinkAllChildrenLocateable();

        //Debug functions

    private:
        point3f_T* location;        //I think this one's fine as a normal pointer -- not intended to be allocated all the time, etc
        Locateable* parent;         //TODO: transition to std::shared_ptr
        Locateable** children;      //TODO: transition to std::shared_ptr * (array of std::shared_ptr 's)
        uint8_t numberOfChildren;       //or maybe the above should be std::weak_ptr ?
};
*/






/*
//Version 0603A: now deprecated(?)
//Class locateable is an abstract base class for objects that have a location
class Locateable {
    public:
        Locateable();
        ~Locateable();

        //Accessors
        point3f_T getLocation();

        //Manipulators
        void setLocation(point3f_T newLocation);
        void moveLocation(point3f_T change);

        //Functions for creating a tree so objects can share locations
        void linkLocation(Locateable& toCarry);
        void unlinkLocation(Locateable& carried);
        void unlinkAllLocateables();

        //Debug functions

    private:
        point3f_T* location;
        Locateable** subLocateables;
        uint8_t numberOfSubLocateables;
        bool readOnly;
        void linkSubLocateables(point3f_T* toLocation);
};
*/


#endif // LOCATEABLE_H
