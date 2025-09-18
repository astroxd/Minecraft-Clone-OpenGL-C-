#ifndef EVENT_H
#define EVENT_H

//#include <iostream>
#include <string>

namespace Events {

	class Event {
	public:
		virtual ~Event() = default;
		virtual std::string GetEventType() const = 0;
		virtual std::string ToString() const { return GetEventType(); }

		bool m_IsHandled{ false };
	};

	/*inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}*/

}

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
