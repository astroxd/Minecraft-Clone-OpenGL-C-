#ifndef WINDOW_EVENT_H
#define WINDOW_EVENT_H

#include "Event.h"

class WindowResizeEvent : public Events::Event {
public:

	EVENT_TYPE("WindowResizeEvent");

	WindowResizeEvent(unsigned int width, unsigned int height)
		: m_Width(width), m_Height(height)
	{
	}

	unsigned int m_Width{ 0 };
	unsigned int m_Height{ 0 };

};

#endif 

