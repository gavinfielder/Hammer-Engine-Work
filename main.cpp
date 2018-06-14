#include <iostream>
#include "Global.h"
#include "Locator.h"
#include "Weapon.h"
#include "HealthTracker.h"
#include <string>
#include "Clock.h"
#include "EventCore.h"

using namespace std;

//Function declarations
void printCurrentTime();
void printLocation(Locator& loc);
void printLocateable(Locator& loc, string name);
void locatorTest1();
void locatorTest2();
void createSomeObjects();
void firingTest1();
void processDebugEvent(short msg);

const short createObjectsMsg = 1;
const short locatorTest1Msg = 2;
const short locatorTest2Msg = 3;
const short firingTest1Msg = 4;
const short exitMsg = -777;


//global variables
Stopwatch testTimer;
HealthTracker practiceDummy;
Weapon rifle;

int main() {
    time_point<high_resolution_clock> clockTest_startTime = high_resolution_clock::now();
    seedRandomNumberGenerator();

    //Event-based architecture test
    printCurrentTime();
    cout << "Proceeding with event-based architecture test.\n";
    EventQueue testEventQueue;
    testEventQueue.addScheduledEvent(*(new Event(-101,createObjectsMsg,30000)));
    //testEventQueue.addScheduledEvent(*(new Event(-101,locatorTest1Msg,8000)));
    //testEventQueue.addScheduledEvent(*(new Event(-101,locatorTest2Msg,12000)));
    testEventQueue.addScheduledEvent(*(new Event(-101,firingTest1Msg,60000)));
    testEventQueue.addScheduledEvent(*(new Event(-101,exitMsg,90000)));
    eventReactionFeedback feedback;

    unsigned int i = 0;
    programTime lastAction = 0, timeSinceLastAction = 0;
    cout << endl;
    cout << "//////////////////////////////////////////////////\n";
    cout << "////////////// Entering program loop /////////////\n";
    cout << "//////////////////////////////////////////////////\n";
    while (true) {
        if (testEventQueue.checkSchedule()) {
            feedback = testEventQueue.processEvent();
            if (feedback.message == -777) break;
            processDebugEvent(feedback.message);
            lastAction = Clock::getTime();
            printCurrentTime();
            cout << endl;
        }
        else if (i % 15 == 0) {
            timeSinceLastAction = Clock::getTime() - lastAction;
            if (timeSinceLastAction >= 11000) {
                cout << "Waiting...\n";
                lastAction = Clock::getTime();
                timeSinceLastAction = 0;
            }
        }
        i++;
    }
    cout << "//////////////////////////////////////////////////\n";
    cout << "////////////// Exiting program loop //////////////\n";
    cout << "//////////////////////////////////////////////////\n";


    cout << "\nDone with event-based architecture test.\n";

    //Conclude Clock test
    high_resolution_clock::time_point clockTest_endTime = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(clockTest_endTime - clockTest_startTime);
    printCurrentTime();
    cout << "Clock Test: raw time of program execution: " << time_span.count() << " seconds.\n";
    cout << "Done with Clock test.\n";


    return 0;
}

//Prints the current program time (milliseconds since start, ideally)
void printCurrentTime() {
    cout << "Current program time is " << Clock::getTime() << endl;
}

//Processes a debug event
void processDebugEvent(short msg) {
    switch (msg) {
    case createObjectsMsg:
        createSomeObjects();
        break;
    case locatorTest1Msg:
        locatorTest1();
        break;
    case locatorTest2Msg:
        locatorTest2();
        break;
    case firingTest1Msg:
        firingTest1();
        break;
    }
}


void printLocation(Locator& loc) {
    cout << loc.getLocation().x << ", " << loc.getLocation().y << ", " << loc.getLocation().z;
}
void printLocateable(Locator& loc, string name) {
    cout << "Locator " << name << " {";
    printLocation(loc);
    cout << "}; " << (int)(loc.getNumberOfChildren()) << " children: (";
    for (uint8_t i = 0; i < loc.getNumberOfChildren(); i++) {
        cout << "{";
        printLocation(loc.getChild(i));
        cout << "}";
    }
    cout << ")\n";
}


//Tests the Locator class with respect to parent/children objects
void locatorTest1() {
    cout << "Proceeding with locatorTest1:\n";
    testTimer.start();
    Locator p1, p2, c1, c2, gc1, gc2, gc3, gc4, gc5, gc6;
    p1.setLocation({1,2,0});
    p2.setLocation({-1,2,0});
    c1.setLocation({2,0,0});
    c2.setLocation({2,1,0});
    gc1.setLocation({3,0,0});
    gc2.setLocation({3,1,0});
    gc3.setLocation({3,2,0});
    gc4.setLocation({3,3,0});
    gc5.setLocation({3,4,0});
    gc6.setLocation({3,5,0});
    printLocateable(p1, "p1");
    printLocateable(p2, "p2");
    printLocateable(c1, "c1");
    printLocateable(c2,"c2");
    printLocateable(gc1,"gc1");
    printLocateable(gc2,"gc2");
    printLocateable(gc3,"gc3");
    printLocateable(gc4,"gc4");
    printLocateable(gc5,"gc5");
    printLocateable(gc6,"gc6");

    //Have c1 pick up gc1 and gc1;
    cout << "\nc1 picking up gc1 and gc2. (Note the locations of gc1 and gc2 change to c1's location)\n";
    c1.linkChildLocator(gc1);
    c1.linkChildLocator(gc2);
    printLocateable(c1,"c1");
    printLocateable(gc1,"gc1");
    printLocateable(gc2,"gc2");

    //Have c2 pick up gc11 and gc12;
    cout << "\nc2 picking up gc3 and gc4.\n";
    c2.linkChildLocator(gc3);
    c2.linkChildLocator(gc4);
    printLocateable(c2,"c2");
    printLocateable(gc3,"gc3");
    printLocateable(gc4,"gc4");

    //Have p1 pick up c1 and c2
    cout << "\np1 picking up c1 and c2. (Note all the 'grandchildren' change locations to p1)\n";
    p1.linkChildLocator(c1);
    p1.linkChildLocator(c2);
    printLocateable(p1,"p1");
    printLocateable(c1,"c1");
    printLocateable(c2,"c2");

    //c1 attempts to move -5
    cout << "\nc1 attempts to move by {-5,0,0}. (should fail because c1 is a child)\n";
    c1.moveLocation({-5,0,0});
    printLocateable(p1,"p1");
    printLocateable(c1,"c1");
    printLocateable(c2,"c2");

    //p1 moves -5
    cout << "\np1 moves by {-5,0,0}. (should succeed and move its entire tree)\n";
    p1.moveLocation({-5,0,0});
    printLocateable(p1,"p1");
    printLocateable(c1,"c1");
    printLocateable(c2,"c2");

    //p1 drops c1
    cout << "\np1 drops c1. (c1 should now  be independent, but be left at p1's location)\n";
    p1.unlinkChildLocator(c1);
    printLocateable(p1,"p1");
    printLocateable(c1,"c1");
    printLocateable(c2,"c2");

    //c1 attempts to move -5
    cout << "\nc1 attempts to move by {-5,0,0}. (should succeed since c1 is no longer a child)\n";
    c1.moveLocation({-5,0,0});
    printLocateable(p1,"p1");
    printLocateable(c2,"c2");
    printLocateable(c1,"c1");

    //p2 attempts to pick up c2
    cout << endl;
    printLocateable(p2,"p2");
    cout << "p2 attempts to pick up c2. (should fail since c2 already has a parent)\n";
    p2.linkChildLocator(c2);
    printLocateable(p1,"p1");
    printLocateable(p2,"p2");
    printLocateable(c1,"c1");
    printLocateable(c2,"c2");

    //p2 attempts to pick up c1
    cout << "\np2 attempts to pick up c1 (should succeed and c1 updates its location to p2's location)\n";
    p2.linkChildLocator(c1);
    printLocateable(p1,"p1");
    printLocateable(p2,"p2");
    printLocateable(c1,"c1");
    printLocateable(c2,"c2");

    //c1 drops all its children (which were gc1 and gc2)
    cout << "\nc1 drops all its children (which were gc1 and gc2)\n";
    c1.unlinkAllChildrenLocators();
    printLocateable(p2,"p2");
    printLocateable(c1,"c1");
    printLocateable(gc1,"gc1");
    printLocateable(gc2,"gc2");
    cout << "p2 moves away by {0,17,0}. (Note gc1 and gc2 should not follow)\n";
    p2.moveLocation({0,17,0});
    printLocateable(p2,"p2");
    printLocateable(c1,"c1");
    printLocateable(gc1,"gc1");
    printLocateable(gc2,"gc2");

    cout << "\nDone with locatorTest1. testTimer says this took " << testTimer.getElapsed() << " x10^-4 seconds.\n";
}

//Creates some objects to use in other tests. Uses global objects.
void createSomeObjects() {
    cout << "Creating rifle and practiceDummy.\n";
    testTimer.start();
    //Creating test Weapon and test Damageable
    practiceDummy.location.setLocation({0,25,0});
    practiceDummy.debug_setHitPoints(1000);
    uint8_t tmp1[] = {127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127};
    practiceDummy.debug_setResist(tmp1);
    practiceDummy.debug_setTargetSize(32);
    practiceDummy.debug_setCriticalHitResistance(0);

    rifle.location.setLocation({0,0,0});
    short tmp2[] = {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    rifle.debug_setDamage(tmp2);
    rifle.debug_setAccuracy(100,200);
    rifle.debug_setRange(30,10,130,230);
    rifle.debug_setStabilityParams(70,100,127);
    rifle.debug_setStability(150);
    rifle.debug_setTargetingSizeParams(20,70,64,255);
    rifle.debug_setCritParams(30, 1.50);
    rifle.debug_setAmmoParams(true, 1000, true, 10, 0, 10);
    rifle.debug_setSetupToFire(false, false);
    rifle.debug_setFireAngleParams(-127,127,-20, -18, 12);

    cout << "\nDone creating rifle and practiceDummy. testTimer says this took " << testTimer.getElapsed() << " x10^-4 seconds.\n";
}


//Tests if Locator linked tree functionality works on derived classes
void locatorTest2() {
    cout << "\nProceeding with Locator test 2: Having practiceDummy pick up rifle.\n";
    testTimer.start();
    practiceDummy.location.linkChildLocator(rifle.location);
    printLocateable(practiceDummy.location,"practiceDummy.location");
    printLocateable(rifle.location,"rifle.location");
    cout << "Moving practiceDummy by {5,4,0}.\n";
    practiceDummy.location.moveLocation({5,4,0});
    printLocateable(practiceDummy.location,"practiceDummy.location");
    printLocateable(rifle.location,"rifle.location");
    cout << "practiceDummy drops rifle.\n";
    practiceDummy.location.unlinkChildLocator(rifle.location);
    cout << "Moving practiceDummy by {-15,-20,0}.\n";
    practiceDummy.location.moveLocation({-15,-20,0});
    printLocateable(practiceDummy.location,"practiceDummy.location");
    printLocateable(rifle.location,"rifle.location");

    cout << "\nDone with Locator test 2. testTimer says this took " << testTimer.getElapsed() << " x10^-4 seconds.\n";
}

//Tests firing weapons at targets and its effects (reducing health)
void firingTest1() {
    cout << "\nProceeding with firing test 1:\n";
    testTimer.start();
    shotResult result;
    for (int i = 0; i < 10; i++) {
        result = rifle.fire(practiceDummy);
        cout << "Rifle shot at practice dummy: Shot #" << (i+1);
        if (i+1 < 10) cout << " ";
        cout << ": ";
        switch (result.success) {
            case shotResultFlag::DID_NOT_FIRE:
                cout << "Did not fire. ";
                break;
            case shotResultFlag::MISS:
                cout << "Miss.         ";
                break;
            case shotResultFlag::HIT:
                cout << "Hit.          ";
                break;
            case shotResultFlag::CRITICAL_HIT:
                cout << "Critical hit. ";
        }
        if (result.destroyed) {
            cout << "The practice dummy has been destroyed.\n";
            break;
        }
        else cout << "Practice dummy HP is now " << practiceDummy.getHP() << "\n";
    }
    cout << "\nDone with firing test 1. testTimer says this took " << testTimer.getElapsed() << " x10^-4 seconds.\n";
}




