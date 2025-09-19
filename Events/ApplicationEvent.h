#ifndef APPLICATION_EVENT_H
#define APPLICATION_EVENT_H

#include "Event.h"
#include "../Block.h"

class BlockPlacedEvent : public Events::Event {
public:

	EVENT_TYPE("BlockPlacedEvent");

	BlockPlacedEvent(const InventoryItem& item)
		: m_Item(item)
	{
	}

	InventoryItem m_Item;

};

class BlockBrokenEvent : public Events::Event {
public:

	EVENT_TYPE("BlockBrokenEvent");

	BlockBrokenEvent()
	{
	}
};

#endif 

