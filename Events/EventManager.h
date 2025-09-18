#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "EventHandler.h"
#include <vector>
#include <unordered_map>
#include "../Log.h"

namespace Events {

	class EventManager {
	public:
		EventManager() = delete;
		// Deleting the copy constructor to prevent copies
		EventManager(EventManager& obj) = delete;
		const EventManager& operator=(const EventManager&) = delete;

		static void Shutdown();

		static void Subscribe(const std::string& eventId, std::unique_ptr<IEventHandlerWrapper>&& handler);
		static void Unsubscribe(const std::string& eventId, const std::string& handlerName);
		static void TriggerEvent(const Event& event);
		static void QueueEvent(std::unique_ptr<Event>&& event);
		static void DispatchEvents();
	private:
		static std::vector<std::unique_ptr<Event>> s_EventsQueue;
		static std::unordered_map <std::string, std::vector<std::unique_ptr<IEventHandlerWrapper>>> s_Subscribers;
	};

	template<typename EventType>
	inline void Subscribe(const EventHandler<EventType>& callback)
	{
		std::unique_ptr<IEventHandlerWrapper> handler = std::make_unique<EventHandlerWrapper<EventType>>(callback);
		EventManager::Subscribe(EventType::GetStaticEventType(), std::move(handler));
	}

	template<typename EventType>
	inline void Unsubscribe(const EventHandler<EventType>& callback)
	{
		const std::string handlerName = callback.target_type().name();
		EventManager::Unsubscribe(EventType::GetStaticEventType(), handlerName);
	}

	inline void TriggerEvent(const Event& triggeredEvent)
	{
		EventManager::TriggerEvent(triggeredEvent);
	}

	inline void QueueEvent(std::unique_ptr<Event>&& queuedEvent)
	{
		EventManager::QueueEvent(std::forward<std::unique_ptr<Event>>(queuedEvent));
	}
}

#endif // !EVENT_MANAGER_H
