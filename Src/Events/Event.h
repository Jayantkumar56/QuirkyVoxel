

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Common/Assert.h"

#include <cinttypes>


namespace Mct {

#define BIT(x) 1 << x

	enum class EventType : uint16_t{
		None = 0,
		WindowCloseEvent,				WindowResizeEvent,				WindowMoveEvent,
		WindowMinimizeEvent,			WindowMaximizeEvent,			WindowToggledFocusEvent,
		MouseEnteredEvent,				MouseExitEvent,					MouseMovedEvent,
		MouseScrolledEvent,				MouseScrolledHEvent,			MouseButtonRepeatEvent,
		MouseButtonPressedEvent,		MouseButtonReleasedEvent,		MouseButtonDblClickEvent,	
		KeyPressedEvent,				KeyReleasedEvent,				KeyRepeatEvent
	};

	enum EventCategory : uint16_t{
		None				= 0,
		WindowEvent			= BIT(0),
		InputEvent			= BIT(1),
		MouseEvent			= BIT(2),
		KeyboardEvent		= BIT(3)
	};

#define CATEGORY_AND_TYPE(category, type)	\
			virtual uint16_t  GetEventCategory()  const noexcept override { return category; }\
			static  EventType GetStaticEventType()      noexcept          { return type;     }\
			virtual EventType GetEventType()      const noexcept override { return type;     }

	class Event {
		friend class EventDispatcher;

	public:
		virtual uint16_t  GetEventCategory() const noexcept = 0;
		virtual EventType GetEventType()     const noexcept = 0;

		bool IsHandled() const noexcept { return m_Handled; }

		bool IsCategory(EventCategory category) const noexcept { return GetEventCategory() & category; }

	private:
		bool m_Handled = false;
	};

}
