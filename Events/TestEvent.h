#ifndef TEST_EVENT_H
#define TEST_EVENT_H

#include "Event.h"

class TestEvent : public Events::Event {
public:

	EVENT_TYPE("TEST");

	TestEvent(int a) : value(a) {}

	int value{ 0 };

};

#endif 

