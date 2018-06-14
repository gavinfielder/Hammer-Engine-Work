#include "Locator.h"
#include "Global.h"
#include <cmath>





//General use functions and operators

//The - operator returns the distance between two point3f_T's
float operator-(point3f_T a, point3f_T b) {
    return (float)pow((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z), 0.5);
}

//The + operator returns the sum of two point3f_T's
point3f_T operator+(point3f_T a, point3f_T b) {
    point3f_T toReturn = {a.x + b.x, a.y + b.y, a.z + b.z};
    return toReturn;
}

//Returns the angle of elevation from observer to target points, represented as follows:
//     0 is horizontal
//     +/- 127 is vertical
int8_t angleOfElevation(point3f_T observer, point3f_T target) {
    float distance = pow((observer.x - target.x)*(observer.x - target.x)
                         + (observer.y - target.y)*(observer.y - target.y), 0.5);
    return (int8_t) (atan((target.z - observer.z) / distance) * 254 / PI);
}





//Member functions: Version 0603B; see below for older versions

//Constructor
Locator::Locator() {
    location = NULL;
    parent = NULL;
    children = NULL;
    numberOfChildren = 0;
}
//Destructor
Locator::~Locator() {
    //if (location != NULL) delete location;       //gotta think about this
}

//Accessor -- recursive: returns the location of the highest parent
point3f_T Locator::getLocation() {
    if (parent != NULL) return parent->getLocation();
    else if (location != NULL) {
        return (*location);
    }
    else {
        return {0,0,0};
    }
}

//Manipulators
void Locator::setLocation(point3f_T newLocation) {
    if (parent == NULL) { //children aren't allowed to set their location
        if (location == NULL) location = new point3f_T;
        *location = newLocation;
    }
}
void Locator::moveLocation(point3f_T change) {
    if (parent == NULL) { //children aren't allowed to change their location
        if (location != NULL) { //just in case location isn't allocated
            *location = *location + change;
        }
    }
}


//Linked tree functions

//Links one object to 'carry' another. The calling object will be the 'carrier'
//    and the passed object will be the 'carried'
void Locator::linkChildLocator(Locator& child) {
    if (child.parent == NULL) { //only unattended children can be picked up, to avoid errors
        //Set the child's parent to me
        child.parent = this;
        //Free the memory of the child's location
        delete (child.location);
        child.location = NULL;
        //Add child to my list of objects I'm carrying
        Locator** newArray = new Locator*[numberOfChildren+1];
        for (uint8_t i = 0; i < numberOfChildren; i++)
            newArray[i] = children[i];
        newArray[numberOfChildren] = &child;
        delete[] children;
        children = newArray;
        numberOfChildren++;
    }
}

//Unlinks a specific object being carried
//    NOTE: this function cannot handle trying to remove a non-existent child or
//          double-linked children; it can only handle the expected case of removing
//          one child that is in the list exactly once
void Locator::unlinkChildLocator(Locator& child) {
    uint8_t i = 0; //i for old array, j for new array
    Locator** newArray;
    uint8_t indexOfUnlinkedChild = 0;
    if (numberOfChildren > 1) newArray = new Locator*[numberOfChildren-1];
    else newArray = NULL;
    //Loop over all objects in existing array
    while (i < numberOfChildren) {
        if (&child == (children[i])) {
            //Found the object to delink, now set it up as its own root
            children[i]->parent = NULL;
            children[i]->setLocation(getLocation()); //set it to my location
            children[i] = NULL; //free the pointer so the child doesn't get caught up in the delete[]
            indexOfUnlinkedChild = i;
        }
        i++;
    }
    //Copy other children into new downsized array
    uint8_t m = 0;
    for (uint8_t k = 0; k < numberOfChildren; k++) {
        if (k != indexOfUnlinkedChild) {
            //Add to new array
            newArray[m] = children[k];
            m++;
        }
    }
    delete[] children;
    children = newArray;
    numberOfChildren--;
}

//Unlinks all objects being carried
void Locator::unlinkAllChildrenLocators() {
    while (numberOfChildren > 0) {
        children[numberOfChildren-1]->parent = NULL;
        children[numberOfChildren-1]->setLocation(getLocation());
        numberOfChildren--;
    }
    delete[] children;
    children = NULL;
}














/*
//Version 0603A: Now deprecated (?)
//Member functions

//Constructor
Locateable::Locateable() {
    location = NULL;
    subLocateables = NULL;
    numberOfSubLocateables = 0;
    readOnly = false;
}
//Destructor
Locateable::~Locateable() {
    //if (location != NULL) delete location;       //gotta think about this
}

//Accessor
point3f_T Locateable::getLocation() {
    if (location != NULL) return *location;
    else return {0,0,0};
}

//Manipulators
void Locateable::setLocation(point3f_T newLocation) {
    if (location != NULL) {
        if (!(readOnly))
            *location = newLocation;
    }
}
void Locateable::moveLocation(point3f_T change) {
    if (location != NULL) {
        if (!(readOnly))
            *location = *location + change;
    }
}


//Linked tree functions

//Links one object to 'carry' another. The calling object will be the 'carrier'
//    and the passed object will be the 'carried'
void Locateable::linkLocation(Locateable& toCarry) {
    //Only link if the item is not read only. This will avoid double-linking errors, among probable others
    if (!(toCarry.readOnly)) {}
        //If toCarry has an independent location, get rid of it
        if (toCarry.location != NULL) delete toCarry.location;
        //Set toCarry's location to be the same as my location
        toCarry.location = location;


        //if toCarry has its own Locateables, set them to my location too



        //Set toCarry to readOnly so it can't change my location
        toCarry.readOnly = true;
        //Add toCarry to my list of objects I'm carrying
        Locateable** newArray = new Locateable*[numberOfSubLocateables+1];
        for (int i = 0; i < numberOfSubLocateables; i++)
            newArray[i] = subLocateables[i];
        newArray[numberOfSubLocateables] = &toCarry;
        delete[] subLocateables;
        subLocateables = newArray;
    }
}

//Recursive function to link all sublocateables in a subtree
//Used by linkLocation above and unlinkLocation below
void Locateable::linkSubLocateables(point3f_T* toLocation) {
    for (uint8_t i = 0; i < toCarry.numberOfSubLocateables; i++) {
        if (subLocateables[i].numberOfSubLocateables > 0) subLocateables[i].linkSubLocateables(toLocation);
        toCarry.subLocateables[i]->location = toLocation;
    }
}

//Unlinks a specific object being carried
void Locateable::unlinkLocation(Locateable& carried) {
    uint8_t i = 0; j = 0; //i for old array, j for new array
    Locateable** newArray;
    if (numberOfSubLocateables > 1) newArray = new Locateable*[numberOfSubLocateables-1];
    else newArray = NULL;
    //Loop over all objects in existing array
    while (i < numberOfSubLocateables) {
        if (&carried != subLocateables[i]) {
            //not the droid we're looking for, copy into new array
            if (j < numberOfSubLocateables-1) newArray[j] = subLocateables[i];
            j++;
        }
        else {
            //Found the object to delink, now set it up as its own root
            subLocateables[i]->location = new point3f_T; //make its location independent
            subLocateables[i]->readOnly = false;
            subLocateables[i]->setLocation(getLocation()); //set it to my location
            subLocateables[i]->linkSubLocateables(&subLocateables[i].location); //set all of its subLocateables
        }
        i++;
    }
}
//Unlinks all objects being carried
void unlinkAllLocateables() {

}
*/












