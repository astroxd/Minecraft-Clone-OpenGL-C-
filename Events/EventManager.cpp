#include "EventManager.h"

namespace Events {

	std::vector<std::unique_ptr<Event>> EventManager::s_EventsQueue;
	std::unordered_map <std::string, std::vector<std::unique_ptr<IEventHandlerWrapper>>> EventManager::s_Subscribers;

	void EventManager::Shutdown()
	{
		s_EventsQueue.clear();
		s_Subscribers.clear();
	}

	void EventManager::Subscribe(const std::string& eventId, std::unique_ptr<IEventHandlerWrapper>&& handler)
	{
		auto subscribers = s_Subscribers.find(eventId);
		if (subscribers != s_Subscribers.end()) {
			auto& handlers = subscribers->second;
			for (auto& it : handlers) {
				if (it->GetType() == handler->GetType()) {
					LOG_ERROR("Attempting to double-register callback");
					return;
				}
			}
			handlers.emplace_back(std::move(handler));
		}
		else {
			s_Subscribers[eventId].emplace_back(std::move(handler));
		}
	}

	void EventManager::Unsubscribe(const std::string& eventId, const std::string& handlerName)
	{
		auto& handlers = s_Subscribers[eventId];
		for (auto it = handlers.begin(); it != handlers.end(); ++it) {
			if (it->get()->GetType() == handlerName) {
				it = handlers.erase(it);
				return;
			}
		}
	}

	void EventManager::TriggerEvent(const Event& event_)
	{
		for (auto& handler : s_Subscribers[event_.GetEventType()]) {
			handler->Exec(event_);
		}
	}

	void EventManager::QueueEvent(std::unique_ptr<Event>&& event)
	{
		s_EventsQueue.emplace_back(std::move(event));
	}

	void EventManager::DispatchEvents()
	{
		for (auto eventIt = s_EventsQueue.begin(); eventIt != s_EventsQueue.end();) {
			if (!eventIt->get()->m_IsHandled) {
				TriggerEvent(*eventIt->get());
				eventIt = s_EventsQueue.erase(eventIt);
			}
			else {
				++eventIt;
			}
		}
	}
}