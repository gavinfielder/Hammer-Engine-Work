#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "EventCore.h"
#include "Locator.h"
#include "HealthTracker.h"
#include "Global.h"

typedef unsigned int objectID_T;

//Forward declaration of all classes in this file:
class gameObjectManagerReference;
class GameObject;
class GameObjectManager;
class GameObjectEntry;
class GameObjectEntry_ptr;
class GameObjectArray;
class GameObjectList;

//Wraps a static pointer to the GameObjectManager
class gameObjectManagerReference {
    public:
        static GameObjectManager* get() { return manager; }
        static void set(GameObjectManager* manager_in) { manager = manager_in; }
    private:
        static GameObjectManager* manager;
};

//GameObject is the master class for all game objects
class GameObject {
    public:
        //Constructor
        GameObject(objectID_T objectID_in) { objectID = objectID_in; }
        //Event processing
        EventQueue eventQueue;
        EventInterpreter eventInterpreter;
        //Accessors
        objectID_T getID() { return objectID; }


        //Locator* getLocator() { return nullptr; }
        //HealthTracker* getHealthTracker() { return nullptr; }

        static gameObjectManagerReference manager;
    protected:
        objectID_T objectID;
};

//GameObjectEntry wraps a GameObject into a container for collections
//    Currently the rest of these classes are set up such that there is
//    only ever one copy of GameObjectEntry per GameObject, and that is
//    stored in the primary array of the GameObjectManager.
class GameObjectEntry {
    public:
        GameObjectEntry(GameObject& obj_in);
        ~GameObjectEntry();
        std::unique_ptr<GameObject>* obj;
};

//GameObjectManager manages a large number of GameObjects
//   Intended for single use only--one GameObjectManager to rule them all
class GameObjectManager {
    public:
        //Constructor
        GameObjectManager();
        //Destructor
        ~GameObjectManager();
        //Event processing
        EventQueue eventQueue;
        EventInterpreter eventInterpreter;

    private:
        //Main data array
        GameObjectEntry* data; //1-d array of GameObjectEntry
        const static unsigned int MAX_OBJECTS_INITIAL = 10000;
        const static unsigned int MAX_OBJECTS_STEP = 10000;
        const static unsigned int REALLOCATION_THRESHOLD = 9000;
        //stores the ID of the last GameObject created
        static objectID_T lastIDCreated;

};

/*
//GameObjectEntry_ptr wraps a smart pointer to GameObjectEntry to avoid errors
class GameObjectEntry_ptr {
    friend class GameObjectManager;
    public:
        //Constructors
        GameObjectEntry_ptr();
        GameObjectEntry_ptr(GameObjectEntry* data_ptr_in);
        //Functionality
        GameObjectEntry* get();
        void set(GameObjectEntry* entry);
        //Gets the current status
        int8_t getStatus() { return status; }
        //status constants
        const static int8_t STATUS_NEVERLINKED = 0;
        const static int8_t STATUS_LINKED = 1;
        const static int8_t STATUS_DELETED = -1;
    private:
        std::shared_ptr<GameObjectEntry> data_ptr;
        int8_t status;
};
*/

/*
//Class GameObjectList manages a collection of GameObjects
//    using a doubly-linked list
class GameObjectList {
    public:
        //Constructors
        GameObjectList();
        //Accessors
        GameObject* getByID(objectID_T id);
        //Manipulators
        void add(GameObject& obj);
        void remove(objectID_T id);

        //Stores a pointer to the GameObjectManager
        static gameObjectManagerReference manager;
    private:
        //Node for doubly-linked list
        typedef struct gameObjectNode_struct {
            GameObjectEntry_ptr* data; //1-d array of GameObjectEntry_ptr
            gameObjectNode_struct* next;
            gameObjectNode_struct* prev;
        } gameObjectNode;
        //First and last nodes
        gameObjectNode* head;    //intended for middle of list
        //Sets the head to be in the middle of the list
        void resetHead();
        //Searches for the ID
        gameObjectNode* search(objectID_T id);
        //total object count
        unsigned int size;
};
*/


/*
//Base class for managing an array of GameObjects
class GameObjectArray_base {
    public:
        //Constructors
        GameObjectArray_base();
        GameObjectArray_base(unsigned int size_in);
        //Destructor
        ~GameObjectArray_base();
        //Accessors
        GameObject* getByIndex(unsigned int index);
        bool isAllocated() { return allocated; }
        unsigned int getObjectCount() { return count; }
        unsigned int getHighIndex() { return highIndex; }
        unsigned int getMaxSize() { return maxSize; }
        //Manipulators
        void add(GameObjectEntry* entry); //for new objects
        void setEntry(unsigned int index, GameObjectEntry* entry);
        void remove(unsigned int index);
        void removeAll();       //clears all entries with remove();
        //Memory Management
        void deallocate();
        void reallocate(unsigned int size_in, bool preserve = true);

    protected:
        GameObjectEntry_ptr* data; //1-d array of GameObjectEntry_ptr
        void remove_noChecks(unsigned int index); //efficient remove();
        unsigned int count;     //total count of GameObjects
        unsigned int highIndex; //highest index in use
        unsigned int maxSize;   //current maximum size
        bool allocated;         //current allocated status

};*/

/*
//Class GameObjectArray manages a sorted array of GameObjects
class GameObjectArray : public GameObjectArray_base {
    public:
        GameObject* getByID(objectID_T id);
        //Manipulators
        void add(GameObjectEntry* entry); //for new objects
        void add(objectID_T id); //for objects already created
        void remove(objectID_T id);
        unsigned int search(objectID_T id); //binary search
        void sort(); //sorts the list (useful for directly-set entries)
        //Removes null entries, reduces highIndex and can make the array of minimal size if wanted
        void crunch(int downsizedPadding = -1);
        //For managing the reference to the GameObjectManager
        static gameObjectManagerReference manager;
    private:
        void sortEntry(unsigned int index); //sorts a single entry
        unsigned int findMySpot(objectID_T id); //determines where to put a single entry
        void makeRoom(unsigned int index); //makes room for a new single entry
};
*/















#endif // GAMEOBJECT_H
