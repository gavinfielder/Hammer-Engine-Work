#include "EventCore.h"
#include <iostream> //for debug printing






//Default constructor for an empty event
Event::Event() {
    scheduledTime = 0;
    identifier = 0;
    msgCode = 0;
}
//Initializing constructor for an event
Event::Event(short ID_in, short msgCode_in, programTime schedule_in) {
    scheduledTime = schedule_in;
    identifier = ID_in;
    msgCode = msgCode_in;
}


//Default constructor for EventQueue
EventQueue::EventQueue() {
    numEventsWaiting = 0;
    numUpcoming = 0;
    scheduled_head = nullptr;
    scheduled_tail = nullptr;
    immediate_head = nullptr;
    immediate_tail = nullptr;
}

//Inserts an event at the end of the immediate queue
void EventQueue::insertImmediate(Event& newEvent) {
    if (numEventsWaiting == 0) {
        //first node
        immediate_head = new eventNode;
        immediate_head->event = std::make_unique<Event>(newEvent);
        immediate_head->link = nullptr;
        immediate_tail = immediate_head;
        numEventsWaiting++;
    }
    else {
        //not the first node, add to end of list
        immediate_tail->link = new eventNode;
        immediate_tail->link->event = std::make_unique<Event>(newEvent);
        immediate_tail->link->link = nullptr;
        immediate_tail = immediate_tail->link;
        numEventsWaiting++;
    }
}

void EventQueue::insertScheduled(Event& newEvent) {
    if (numUpcoming == 0) {
        //first node
        scheduled_head = new eventNode;
        scheduled_head->event = std::make_unique<Event>(newEvent);
        scheduled_head->link = nullptr;
        scheduled_tail = scheduled_head;
        numUpcoming++;
    }
    else {
        //not the first node, insert it in the proper place based on order
        if (scheduled_head->event.get()->schedule() > newEvent.schedule()) {
            //making a new head
            eventNode* temp = scheduled_head;
            scheduled_head = new eventNode;
            scheduled_head->event = std::make_unique<Event>(newEvent);
            scheduled_head->link = temp;
            numUpcoming++;
        }
        else {
            //Find the node right before we need to insert
            eventNode* current = scheduled_head;
            while (current->link != nullptr
                   && current->link->event.get()->schedule() < newEvent.schedule()) {
                current = current->link;
            }
            //insert the new node
            eventNode* temp = current->link;
            current->link = new eventNode;
            current->link->event = std::make_unique<Event>(newEvent);
            current->link->link = temp;
            //update the tail if it was at the end
            if (current->link->link == nullptr) scheduled_tail = current->link;
            numUpcoming++;
        }
    }
}

//Transfers the head of the scheduled queue to the tail of the immediate queue
//    Note: this assumes the scheduled queue is nonempty
void EventQueue::kickScheduled() {
    if (numEventsWaiting == 0) {
        //first node
        immediate_head = scheduled_head;
        scheduled_head = scheduled_head->link;
        immediate_tail = immediate_head;
        immediate_tail->link = nullptr;
        numUpcoming--;
        numEventsWaiting++;
        if (numUpcoming == 0) {
            scheduled_head = nullptr;
            scheduled_tail = nullptr;
        }
    }
    else {
        //Not the first node, add to end
        immediate_tail->link = scheduled_head;
        scheduled_head = scheduled_head->link;
        immediate_tail->link = nullptr;
        numUpcoming--;
        numEventsWaiting++;
        if (numUpcoming == 0) {
            scheduled_head = nullptr;
            scheduled_tail = nullptr;
        }
    }
}

//Moves all scheduled events whose time has come to the immediate queue
bool EventQueue::checkSchedule() {
    while (numUpcoming > 0 && scheduled_head->event.get()->schedule() <= Clock::getTime())
        kickScheduled();
    return (numEventsWaiting > 0);
}




//some simple test event reactions
void debug_test_immediateEvent(short msg) {
    std::cout << "Immediate event being processed. ";
    std::cout << "Message: " << msg << ", execution time: " << Clock::getTime() << std::endl;
}
void debug_test_scheduledEvent(short msg) {
    std::cout << "Scheduled event being processed. ";
    std::cout << "Message: " << msg << ", execution time: " << Clock::getTime() << std::endl;
}
void debug_test_loopBreakerEvent(short msg) {
    std::cout << "A loop breaker event is being processed. ";
    std::cout << "Message: " << msg << ", execution time: " << Clock::getTime() << std::endl;

}
void debug_test_eventThatNeverHappens(short msg) {
    std::cout << "An event that shouldn't be processed is being processed. ";
    std::cout << "Message: " << msg << ", execution time: " << Clock::getTime() << std::endl;
}


//Takes an appropriate action based on an event
eventReactionFeedback EventInterpreter::process(Event& event) {
    eventReactionFeedback toReturn;
    switch (event.getID()) {
        case -101:
            //Debug event
            toReturn.timeOfExecution = Clock::getTime();
            debug_test_scheduledEvent(event.getMsgCode());
            break;
    }
    toReturn.identifier = event.getID();
    toReturn.message = event.getMsgCode();
    return toReturn;
}



//Processes a single event from the immediate queue
//    NOTE: Assumes the immediate queue is nonempty
eventReactionFeedback EventQueue::processEvent() {
    Event* event = immediate_head->event.get();
    //Process event
    eventReactionFeedback toReturn = EventInterpreter::process((*event)); //toReturn is an event identifier right now, might want a custom struct for this instead
    //Remove from queue
    numEventsWaiting--;
    if (numEventsWaiting == 0) {
        immediate_head = nullptr;
        immediate_tail = nullptr;
    }
    else
        immediate_head = immediate_head->link;
    delete event; //TODO: This seems to work, but shouldn't I be deleting using the smart pointer's functionality? To investigate.
    return toReturn;
}

//Processes all events in the immediate queue
void EventQueue::processAllWaiting() {
    while (numEventsWaiting > 0)
        processEvent();
}


//Public function for adding an immediate event
void EventQueue::addImmediateEvent(Event& event) {
    insertImmediate(event);
}

//Public function for adding a scheduled event
void EventQueue::addScheduledEvent(Event& event) {
    insertScheduled(event);
}













