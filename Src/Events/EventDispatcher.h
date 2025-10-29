

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Event.h"

#include <concepts>


namespace Mct {

	typedef void (*EventCallbackFn)(Event&, void*);

	class EventDispatcher {
	public:
		static void RegisterEventCallback(EventCallbackFn callback,
										  void* userData = nullptr) noexcept
		{
			s_EventCallbackFn = callback;
			s_UserData        = userData;
		}

		// Propagate an event to the Event callback.
		static void DispatchEvent(Event& event) {
			MCT_ASSERT(s_EventCallbackFn != nullptr &&
				"Event callback is not registered yet! "
				"Forgot to call RegisterEventCallback before DispatchEvent.");

			s_EventCallbackFn(event, s_UserData);
		}

		// Handles certain type of event.
		template<typename EventT, typename Function>
			requires std::derived_from<EventT, Event> && std::invocable<Function&, EventT&>
		static bool Handle(Event& event, Function&& func)
				noexcept(noexcept(std::invoke(std::forward<Function>(func), static_cast<EventT&>(event))))
		{
			if (event.IsHandled()) return true;
			if (event.GetEventType() != EventT::GetStaticEventType()) return false;

			event.m_Handled |= std::invoke(std::forward<Function>(func), static_cast<EventT&>(event));
			return event.m_Handled;
		}

	private:
		static inline void* s_UserData = nullptr;
		static inline EventCallbackFn s_EventCallbackFn = nullptr;
	};

}
