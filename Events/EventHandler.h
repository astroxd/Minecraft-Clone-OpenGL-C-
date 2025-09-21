#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "Event.h"
#include <functional>

namespace Events {

	template<typename EventType>
	using EventHandler = std::function<void(const EventType& e)>;

	class IEventHandlerWrapper {
	public:

		virtual ~IEventHandlerWrapper() = default;

		void Exec(const Event& e) {
			Call(e);
		}

		virtual std::string GetType() const = 0;

	private:
		virtual void Call(const Event& e) = 0;
	};

	template<typename EventType>
	class EventHandlerWrapper : public IEventHandlerWrapper {
	public:
		explicit EventHandlerWrapper(const EventHandler<EventType>& handler)
			: m_Handler(handler)
			, m_HandlerType(m_Handler.target_type().name()) {
		};

	private:
		void Call(const Event& e) override
		{
			if (e.GetEventType() == EventType::GetStaticEventType()) {
				m_Handler(static_cast<const EventType&>(e));
			}
		}

		std::string GetType() const override { return m_HandlerType; }

		EventHandler<EventType> m_Handler;
		const std::string m_HandlerType;
	};
}
#endif
