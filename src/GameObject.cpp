#include "GameObject.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////// GameObjectEntry ///////////////////////////////
//////////////////////////////////////////////////////////////////////////

//Constructor
GameObjectEntry::GameObjectEntry(GameObject& obj_in) {
    *obj = std::make_unique<GameObject>(obj_in);
}

//Destructor
GameObjectEntry::~GameObjectEntry() {
    delete obj; //I'm assuming this will delete the underlying object as well
}

/*
//////////////////////////////////////////////////////////////////////////
//////////////////////// GameObjectEntry_ptr /////////////////////////////
//////////////////////////////////////////////////////////////////////////

//Default constructor for empty entry
GameObjectEntry_ptr::GameObjectEntry_ptr() {
    data_ptr = nullptr;
    status = STATUS_NEVERLINKED;
}

//Initializing constructor
GameObjectEntry_ptr::GameObjectEntry_ptr(GameObjectEntry* data_ptr_in) {
    data_ptr = data_ptr_in;
    status = STATUS_LINKED;
}

//Gets the pointer
GameObjectEntry* GameObjectEntry_ptr::get() {
    if (status == STATUS_DELETED) return nullptr;
    return data_ptr;
}

//Sets the pointer, if it's not yet been set
void GameObjectEntry_ptr::set(GameObjectEntry* entry) {
    if (status==STATUS_NEVERLINKED)
        data_ptr = entry;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////// GameObjectArray_base ////////////////////////////
//////////////////////////////////////////////////////////////////////////

//Deallocates the array of pointers but does not delete the underlying data
void GameObjectArray_base::deallocate() {
    if (allocated) {
        delete[] data;
        data = nullptr;
        allocated = false;
    }
}

//Allocates the array of pointers
void GameObjectArray_base::reallocate(unsigned int size_in, bool preserve) {
    if ((preserve == false) || (allocated == false)) {
        //must reference base class specifically because a derived class
        //   overrides deallocate such that it deletes the underlying data,
        //   which we may not want.
        GameObjectArray_base::deallocate();
        maxSize = size_in;
        data = new GameObjectEntry* [maxSize];
        allocated = true;
        removeAll();
    }
    else {
        //preserve existing data
        GameObjectEntry** temp = new GameObjectEntry* [size_in];
        for (unsigned int i = 0; i < highIndex; i++)
            temp[i] = data[i];
        //Transfer temp over
        delete[] data;
        maxSize = size_in;
        data = temp;
        //Clean the empty data
        for (unsigned int i = highIndex; i < maxSize; i++)
            remove_noChecks(i);
        //allocated is already true
    }
}

//Default constructor for deallocated array
GameObjectArray_base::GameObjectArray_base() {
    data = nullptr;
    allocated = false;
    count = 0;
    highIndex = 0;
    maxSize = 0;
}

//Constructor for allocated array
GameObjectArray_base::GameObjectArray_base(unsigned int size_in) {
    allocated = false;
    reallocate(size_in,false);
    count = 0;
    highIndex = 0;
}

//Destructor: does not destroy underlying data
GameObjectArray_base::~GameObjectArray_base() {
    deallocate();
}

//Gets the GameObject at the specified index
GameObject* GameObjectArray_base::getByIndex(unsigned int index) {
    if (allocated) {
        if (index <= highIndex) {
            if (data[index] != nullptr) {
                return data[index]->obj->get();
            }
        }
    }
    return nullptr;
}

//Adds an entry pointer directly
void GameObjectArray_base::add(GameObjectEntry* entry) {
    if (highIndex + 1 <= maxSize) {
        highIndex++;
        data[highIndex] = entry;
    }
}

//Sets an entry pointer directly
void GameObjectArray_base::setEntry(unsigned int index, GameObjectEntry* entry) {
    if (index < maxSize) {
        data[index] = entry;
    }
}

//Clears the specified entry pointer -- does not delete underlying data
void GameObjectArray_base::remove(unsigned int index) {
    if (allocated) {
        if (index < maxSize)
            remove_noChecks(index);
    }
}

//Efficient remove without safeguards
void GameObjectArray_base::remove_noChecks(unsigned int index) {
    data[index] = nullptr;
}

//Clears all entries
void GameObjectArray_base::removeAll() {
    if (allocated) {
        for (unsigned int i = 0; i < maxSize; i++)
            remove_noChecks(i);
    }
}


//////////////////////////////////////////////////////////////////////////
/////////////////////////// GameObjectArray //////////////////////////////
//////////////////////////////////////////////////////////////////////////



//Searches the array using a binary search; assumes list is sorted
unsigned int GameObjectArray::search(objectID_T id) {
    if (allocated) {
        unsigned int a = 0, b = highIndex, c;
        short j = 1;
        bool found = false;
        objectID_T thisID;
        while (a <= b && !found) {
            c = (a + b) / 2;
            //make sure c isn't a null reference
            if (data[c] == nullptr) {
                j = 1;
                while (true) {
                    if ((c-j >= 0) && (data[c-j] != nullptr)) {
                        c = c-j;
                        break;
                    }
                    else if ((c+j <= highIndex) && (data[c+j] != nullptr)) {
                        c = c+j;
                        break;
                    }
                    else if ((c-j < 0) && (c+j > highIndex)) {
                        //it's a null array. Item not found
                        return maxSize; //this flags not found
                    }
                    j++;
                }
            }
            //Retrieve ID to compare against
            thisID = data[c]->obj->get()->getID();
            if (thisID == id)
                found = true;
            else if (id < thisID)
                b = c - 1;
            else
                a = c + 1;
        }
        if (found) return c;
        else return maxSize; //this flags not found
    }
    return maxSize; //this flags not found
}

//returns the highest sorted index whose entries are less than a specified ID
//    i.e., either finds the highest index such that the array up to that
//    index is sorted, or returns the index such that all entries up to that
//    index have IDs less than the given ID (Private function)
unsigned int GameObjectArray::findMySpot(objectID_T id) {
    unsigned int index = 0, j = 1;
    objectID_T thisID, nextID;
    //Initialize thisID
    if (data[0] != nullptr) thisID = data[0]->obj->get()->getID();
    else thisID = 0;
    //Search loop
    while (index <= highIndex) {
        j = 1; //j marks how many spaces ahead to look (for skipping null entries)
        if (data[index] != nullptr) {
            //Find a next object to compare to
            if (index + j < maxSize) {
                while ((data[index+j] == nullptr) && (index+j <= highIndex))
                    j++;
            }
            if (index + j == highIndex + 1) //All entries beyond index are empty
                return (index + 1);
            //Set nextID to the ID to compare against
            nextID = data[index+j]->obj->get()->getID();
            //Now compare the two ID's
            if (thisID > nextID) //not sorted. Found the highest sorted index
                return (index + 1);
            if (id <= nextID)    //found the proper spot
                return (index + 1);
            //Not yet found the spot, move on
            thisID = nextID;
        }
        //Move the index to the next nonempty index
        index += j;
    }
    return index;
}

//Shunts all the entries after a particular index by one (Private function)
void GameObjectArray::makeRoom(unsigned int index) {
    unsigned int i = index;
    //Find the first empty slot after index
    while (i < maxSize && data[i] != nullptr)
        i++;
    //If array is full, return without doing anything
    if (i == maxSize) return;
    if (i == highIndex + 1) highIndex++;
    //Shunt all entries forward
    for (; i > index && i > 0; i--)
        data[i] = data[i-1];
    //Clean the new single space
    data[index] = nullptr;
}

//Takes an entry and makes sure its in its proper place among
//    all the entries before it (Private function)
void GameObjectArray::sortEntry(unsigned int index) {
    unsigned int shouldBe = findMySpot(data[index]->obj->get()->getID());
    if (index == shouldBe) return;
    //else we need to sort this entry. First, take it out
    GameObjectEntry* temp = data[index];
    data[index] = nullptr;
    //Then put it where it should be
    makeRoom(shouldBe);
    data[shouldBe] = temp;
}

//Sorts the entire array
void GameObjectArray::sort() {
    if (allocated && maxSize > 1) {
        unsigned int index = 1;
        while (index <= highIndex) {
            sortEntry(index);
            index++;
        }
    }
}

//Removes null spaces and reallocates with the specified # of free spaces
//    at the end, if downsizedPadding is nonnegative
void GameObjectArray::crunch(int downsizedPadding) {
    if (allocated) {
        unsigned int index = 0, j = 0;
        while (true) {
            //Find the next nonempty entry
            while ((index+j <= highIndex) && (data[index+j] == nullptr))
                j++;
            if (index+j > highIndex) break;
            //copy data
            data[index] = data[index+j];
            //increment index
            index++;
        }
        //set new highIndex
        if (index > 0) highIndex = index - 1;
        else highIndex = 0;
        //Resize if requested
        if (downsizedPadding >= 0)
            reallocate(highIndex + 1 + downsizedPadding, true);
    }
}

//Removes the specified ID
void GameObjectArray::remove(objectID_T id) {
    if (allocated) {
        unsigned int index = search(id);
        if (index == maxSize) //item not found
            return;

    }
}

*/













