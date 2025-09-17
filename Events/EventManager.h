#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "EventHandler.h"
#include <vector>
#include <unordered_map>
#include "../Log.h"

class EventManager {
public:
	EventManager() = default;
	void Shutdown();

	void Subscribe(const std::string& eventId, std::unique_ptr<EventHandlerWrapperInterface>&& handler);
	void Unsubscribe(const std::string& eventId, const std::string handlerName);
	void TriggerEvent(const Event& event);
	void QueueEvent(std::unique_ptr<Event>&& event);
	void DispatchEvents();
private:
	std::vector<std::unique_ptr<Event>> m_eventsQueue;
	std::unordered_map <std::string, std::vector<std::unique_ptr<EventHandlerWrapperInterface>>> m_subscribers;
};

extern EventManager gEventManager;

template<typename EventType>
inline void Subscribe(const EventHandler<EventType>& callback)
{
	std::unique_ptr<EventHandlerWrapperInterface> handler = std::make_unique<EventHandlerWrapper<EventType>>(callback);
	gEventManager.Subscribe(EventType::GetStaticEventType(), std::move(handler));
}

template<typename EventType>
inline void Unsubscribe(const EventHandler<EventType>& callback)
{
	const std::string handlerName = callback.target_type().name();
	gEventManager.Unsubscribe(EventType::GetStaticEventType(), handlerName);
}

inline void TriggerEvent(const Event& triggeredEvent)
{
	gEventManager.TriggerEvent(triggeredEvent);
}

inline void QueueEvent(std::unique_ptr<Event>&& queuedEvent)
{
	gEventManager.QueueEvent(std::forward<std::unique_ptr<Event>>(queuedEvent));
}





#endif // !EVENT_MANAGER_H
