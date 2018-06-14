#include "Global.h"
#include <cstdlib>
#include <ctime>
#include <cstdint>

//Seeds the random number generator
void seedRandomNumberGenerator() {
    srand(time(NULL));
}

//Creates an energy_T struct type
energy_T newEnergy(short value, char order) {
    energy_T toReturn;
    toReturn.value = value;
    toReturn.order = order;
    return toReturn;
}
