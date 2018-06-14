#ifndef EVENTCORE_H
#define EVENTCORE_H

#include "Clock.h"
#include <memory>

//Class Event is used as a base class for events
class Event {
    public:
        //Constructors
        Event();
        Event(short ID_in, short msgCode_in, programTime schedule_in = 0);
        //Accessors
        programTime schedule() { return scheduledTime; }
        short getID() { return identifier; }
        short getMsgCode() { return msgCode; }
    private:
        //Stores the scheduled time of the event. Immediate events are scheduled at 0
        programTime scheduledTime;
        //Stores an identifying integer
        short identifier;
        //Stores a message integer
        short msgCode;
        //TODO: add pointers to the objects that call them
};



//Used for passing back to objects generating events, etc.
typedef struct eventReactionFeedback_struct {
    programTime timeOfExecution;
    short identifier;
    short message;
} eventReactionFeedback;


//Class EventInterpreter parses identifiers and msgCodes if there is a need for it
//TODO: revise as per new architecture: every GameObject has its own EventInterpreter
class EventInterpreter {
    public:
        static eventReactionFeedback process(Event& event); //TODO: should not be static
};

//EventQueue holds two stacks of events: immediate events and scheduled events
class EventQueue {
    public:
        //Constructors
        EventQueue();
        //Adds an event to be queued immediately
        void addImmediateEvent(Event& newEvent);
        //Adds an event to be queued at/after the specified time
        void addScheduledEvent(Event& newEvent);
        //Reviews all scheduled events and moves them to immediate events queue if they're ready
        //   returns true if there are any immediate events waiting after the check
        bool checkSchedule();
        //Returns the number of Events in the respective queue
        unsigned short numberOfEventsWaiting() { return numEventsWaiting; }
        unsigned short numberOfEventsUpcoming() { return numUpcoming; }
        //Processes the event at the top of the immediate queue
        //    NOTE: Assumes immediate queue is nonempty
        eventReactionFeedback processEvent();
        //Processes all the events in the immediate queue
        //    Not ideal since this one doesn't give feedback
        void processAllWaiting();
        //For managing the EventInterpreter link
        EventInterpreter* getEventInterpreter() { return eventInterpreter; }
        void setEventInterpreter(EventInterpreter* inter) { eventInterpreter = inter; }

    private:
        //Node structure for EventQueue linked lists
        typedef struct eventNode_struct {
            std::unique_ptr<Event> event;
            eventNode_struct* link;
        } eventNode;
        //Number of events in the immediate queue
        unsigned short numEventsWaiting;
        //Number of events in the scheduled queue
        unsigned short numUpcoming;
        //Refers to first and last node in each linked list
        eventNode* scheduled_head;
        eventNode* scheduled_tail;
        eventNode* immediate_head;
        eventNode* immediate_tail;
        //insert functions
        void insertScheduled(Event& newEvent); //sorts by scheduled time
        void insertImmediate(Event& newEvent); //adds to end of immediate queue
        //Transfers the head of the scheduled queue to the end of the immediate queue
        void kickScheduled();
        //Stores address of a corresponding EventInterpreter
        EventInterpreter* eventInterpreter;
};


//EventManager tracks and prioritizes a large number of event queues
class EventManager {
    public:

    private:


};

























#endif // EVENTCORE_H




















