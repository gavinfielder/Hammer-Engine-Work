#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

using namespace std::chrono;

typedef unsigned int programTime;

//Class clock wraps timing functionality into a generalized int
class Clock {
    public:
        static programTime getTime();
        const static time_point<high_resolution_clock> getProgramStartTime() { return programStartTime; }

    private:
        //This implementation could change
        const static time_point<high_resolution_clock> programStartTime;
};


//Class timer
class Timer {
    public:
        //Constructor
        Timer();
        //Starts the timer
        void start(programTime programTimeTarget_in);
        //Checks if scheduled time has been reached and throws event if it has
        bool isDone();
    private:
        programTime programTimeTarget;
        bool active;
        bool done;
        //This implementation could change
        time_point<high_resolution_clock> startTime;
};

//Class Stopwatch times events for informational purposes
class Stopwatch {
    public:
        //'starts' the stopwatch by storing the current programTime into startTime
        void start();
        //returns the time elapsed since the program Start
        programTime getElapsed();
    private:
        programTime startTime;
};



#endif // CLOCK_H
