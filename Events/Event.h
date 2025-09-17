#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <string>

class Event {
public:
	virtual std::string GetEventType() const = 0;

	bool isHandled{ false };
};

#define EVENT_TYPE(event_type)                  \
    static std::string GetStaticEventType()     \
    {                                           \
        return event_type;                      \
    }                                           \
    std::string GetEventType() const override   \
    {                                           \
        return GetStaticEventType();            \
    }



#endif 
