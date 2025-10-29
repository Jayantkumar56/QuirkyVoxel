

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Event.h"


namespace Mct {

	class KeyPressedEvent final : public Event {
	public:
		KeyPressedEvent(const int keyCode, const int mods) noexcept : 
				m_KeyCode ( keyCode ),
				m_Mods    ( mods    )
		{}

		int GetKeyCode() const noexcept { return m_KeyCode; }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::KeyboardEvent, EventType::KeyPressedEvent);

	private:
		int m_Mods;
		int m_KeyCode;
	};

	class KeyRepeatEvent final : public Event {
	public:
		KeyRepeatEvent(const int keyCode, const int mods) noexcept :
				m_KeyCode ( keyCode ),
				m_Mods    ( mods    )
		{}

		int GetKeyCode() const noexcept { return m_KeyCode; }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::KeyboardEvent, EventType::KeyRepeatEvent);

	private:
		int m_Mods;
		int m_KeyCode;
	};

	class KeyReleasedEvent final : public Event {
	public:
		KeyReleasedEvent(const int keyCode, const int mods) noexcept :
				m_KeyCode ( keyCode ),
				m_Mods    ( mods    )
		{}

		int GetKeyCode() const noexcept { return m_KeyCode; }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::KeyboardEvent, EventType::KeyPressedEvent);

	private:
		int m_Mods;
		int m_KeyCode;
	};

}
