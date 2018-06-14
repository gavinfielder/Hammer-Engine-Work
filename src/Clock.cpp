#include "Clock.h"
#include <chrono>

using namespace std::chrono;


const time_point<high_resolution_clock> Clock::programStartTime = high_resolution_clock::now();

//Retrieves the current time
programTime Clock::getTime() {
    duration<double> time_span = duration_cast<duration<double>>(high_resolution_clock::now() - programStartTime);
    return ((programTime)(time_span.count() * 10000));
}


//Constructor
Timer::Timer() {
    startTime = Clock::getProgramStartTime();
    active = false;
    done = false;
}

//Starts the timer to flag event when program reaches the specified time
void Timer::start(programTime programTimeTarget_in) {
    programTimeTarget = programTimeTarget_in;
    startTime = high_resolution_clock::now();
    active = true;
    done = false;
}

//Checks if the specified time has been reached
bool Timer::isDone() {
    if (done) return true;
    if (active) {
        if (Clock::getTime() >= programTimeTarget) {
            active = false;
            done = true;
            return true;
        }
    }
    return false;
}

//Starts the stopwatch
void Stopwatch::start() {
    startTime = Clock::getTime();
}

//Returns the time elapsed
programTime Stopwatch::getElapsed() {
    return (Clock::getTime() - startTime);
}






















