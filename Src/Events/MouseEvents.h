

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Event.h"


namespace Mct {

	class MouseMovedEvent final : public Event {
	public:
		MouseMovedEvent(const double xPrevPos, const double yPrevPos, const double xPos, const double yPos) noexcept :
				m_PrevPosX ( xPrevPos ),
				m_PrevPosY ( yPrevPos ),
				m_PosX     ( xPos     ),
				m_PosY     ( yPos     ) 
		{}

		double GetDeltaX() const noexcept { return m_PosX - m_PrevPosX; }
		double GetDeltaY() const noexcept { return m_PosY - m_PrevPosY; }
		double GetPosX()   const noexcept { return m_PosX;              }
		double GetPosY()   const noexcept { return m_PosY;              }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::MouseEvent, EventType::MouseMovedEvent);

	private:
		double m_PrevPosX;
		double m_PrevPosY;
		double m_PosX;
		double m_PosY;
	};

	class MouseButtonDblClickEvent final : public Event {
	public:
		MouseButtonDblClickEvent(const int button, const double xPos, const double yPos) noexcept :
				m_Button ( button ),
				m_PosX   ( xPos   ),
				m_PosY   ( yPos   )
		{}

		int    GetButton() const noexcept { return m_Button; }
		double GetPosX()   const noexcept { return m_PosX;   }
		double GetPosY()   const noexcept { return m_PosY;   }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::MouseEvent, EventType::MouseButtonDblClickEvent);

	private:
		int    m_Button;
		double m_PosX;
		double m_PosY;
	};

	class MouseButtonPressedEvent final : public Event {
	public:
		MouseButtonPressedEvent(const int button, const int mods, const double xPos, const double yPos) noexcept :
				m_Button ( button ),
				m_Mods   ( mods   ),
				m_PosX   ( xPos   ),
				m_PosY   ( yPos   )
		{}

		int    GetButton() const noexcept { return m_Button; }
		double GetPosX()   const noexcept { return m_PosX;   }
		double GetPosY()   const noexcept { return m_PosY;   }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::MouseEvent, EventType::MouseButtonPressedEvent);

	private:
		int    m_Button;
		int    m_Mods;
		double m_PosX;
		double m_PosY;
	};

	class MouseButtonRepeatEvent final : public Event {
	public:
		MouseButtonRepeatEvent(const int button, const double xPos, const double yPos) noexcept :
				m_Button ( button ),
				m_PosX	 ( xPos   ),
				m_PosY	 ( yPos   )
		{}

		int    GetButton() const noexcept { return m_Button; }
		double GetPosX()   const noexcept { return m_PosX;   }
		double GetPosY()   const noexcept { return m_PosY;   }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::MouseEvent, EventType::MouseButtonRepeatEvent);

	private:
		int    m_Button;
		double m_PosX;
		double m_PosY;
	};

	class MouseButtonReleasedEvent final : public Event {
	public:
		MouseButtonReleasedEvent(const int button, const int mods, const double xPos, const double yPos) noexcept :
				m_Button ( button ),
				m_Mods   ( mods   ),
				m_PosX   ( xPos   ), 
				m_PosY   ( yPos   )
		{}

		int    GetButton() const noexcept { return m_Button; }
		double GetPosX()   const noexcept { return m_PosX;   }
		double GetPosY()   const noexcept { return m_PosY;   }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::MouseEvent, EventType::MouseButtonReleasedEvent);

	private:
		int    m_Button;
		int    m_Mods;
		double m_PosX;
		double m_PosY;
	};

	class MouseScrolledEvent final : public Event {
	public:
		MouseScrolledEvent(const double xoffset, const double yoffset, const double posX, const double posY) noexcept :
				m_XOffset ( xoffset ),
				m_YOffset ( yoffset ),
				m_PosX    ( posX    ),
				m_PosY    ( posY    )
		{}

		double GetXOffset() const noexcept { return m_XOffset; }
		double GetYOffset() const noexcept { return m_XOffset; }
		double GetPosX()    const noexcept { return m_PosX;    }
		double GetPosY()    const noexcept { return m_PosY;    }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::MouseEvent, EventType::MouseScrolledEvent);

	private:
		double m_XOffset;
		double m_YOffset;
		double m_PosX;
		double m_PosY;
	};

}
