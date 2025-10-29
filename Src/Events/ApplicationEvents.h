

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Event.h"


namespace Mct {

	class WindowCloseEvent final : public Event {
	public:
		WindowCloseEvent() noexcept = default;
		CATEGORY_AND_TYPE(EventCategory::WindowEvent, EventType::WindowCloseEvent);
	};

	class WindowResizeEvent final : public Event {
	public:
		WindowResizeEvent(const int width, const int height) noexcept :
				m_Width  ( width  ), 
				m_Height ( height ) 
		{}

		int GetWidth()  const noexcept { return m_Width;  }
		int GetHeight() const noexcept { return m_Height; }

		CATEGORY_AND_TYPE(EventCategory::WindowEvent, EventType::WindowResizeEvent);

	private:
		int m_Width;
		int m_Height;
	};

	class WindowMoveEvent final : public Event {
	public:
		WindowMoveEvent(const int xPos, const int yPos) noexcept :
				m_PosX(xPos),
				m_PosY(yPos)
		{}

		uint32_t GetPosX() const noexcept { return m_PosX; }
		uint32_t GetPosY() const noexcept { return m_PosY; }

		CATEGORY_AND_TYPE(EventCategory::WindowEvent, EventType::WindowMoveEvent);

	private:
		int m_PosX;
		int m_PosY;
	};

	class WindowMinimizeEvent final : public Event {
	public:
		WindowMinimizeEvent() noexcept = default;
		CATEGORY_AND_TYPE(EventCategory::WindowEvent, EventType::WindowMinimizeEvent);
	};

	class WindowMaximizeEvent final : public Event {
	public:
		WindowMaximizeEvent() noexcept = default;
		CATEGORY_AND_TYPE(EventCategory::WindowEvent, EventType::WindowMaximizeEvent);
	};

	class WindowToggledFocusEvent final : public Event {
	public:
		WindowToggledFocusEvent(const bool isFocused) noexcept :
				m_IsFocused(isFocused) 
		{}

		bool GetIsFocused() const noexcept { return m_IsFocused; }

		CATEGORY_AND_TYPE(EventCategory::WindowEvent, EventType::WindowToggledFocusEvent);

	private:
		bool m_IsFocused;
	};

}
