

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Utils/Assert.h"

#include <cstdint>
#include <string_view>
#include <string>
#include <span>
#include <vector>


namespace Mct {

	enum class ShaderDataType {
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	[[nodiscard]] constexpr uint32_t ShaderDataTypeSize(const ShaderDataType type) noexcept {
		switch (type) {
			case ShaderDataType::Float:    return 4;
			case ShaderDataType::Float2:   return 4 * 2;
			case ShaderDataType::Float3:   return 4 * 3;
			case ShaderDataType::Float4:   return 4 * 4;
			case ShaderDataType::Mat3:     return 4 * 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4 * 4;
			case ShaderDataType::Int:      return 4;
			case ShaderDataType::Int2:     return 4 * 2;
			case ShaderDataType::Int3:     return 4 * 3;
			case ShaderDataType::Int4:     return 4 * 4;
			case ShaderDataType::Bool:     return 1;
			
			default: MCT_ASSERT(false && "Unknown ShaderDataType!");
		}

		return 0;
	}

	[[nodiscard]] constexpr uint32_t GetComponentCount(const ShaderDataType type) noexcept {
		switch (type) {
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3 * 3;
			case ShaderDataType::Mat4:    return 4 * 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;

			default: MCT_ASSERT(false && "Unknown ShaderDataType!");
		}
		
		return 0;
	}

	struct BufferElement {
		std::string    Name;
		ShaderDataType Type;

		BufferElement(const ShaderDataType type, std::string name) 
			noexcept(std::is_nothrow_move_constructible_v<std::string>) :
				Name ( std::move(name) ),
				Type ( type            ) 
		{}

		[[nodiscard]] constexpr bool IsIntType() const noexcept {
			return Type == ShaderDataType::Int  || Type == ShaderDataType::Int2 || 
				   Type == ShaderDataType::Int3 || Type == ShaderDataType::Int4;
		}

		[[nodiscard]] constexpr bool IsFloatType() const noexcept {
			return Type == ShaderDataType::Float  || Type == ShaderDataType::Float2 || 
				   Type == ShaderDataType::Float3 || Type == ShaderDataType::Float4;
		}
	};

	class BufferLayout {
	public:
		BufferLayout(std::initializer_list<BufferElement> elements) : 
				m_Elements(elements)
		{
			for (auto& element : m_Elements) {
				m_Stride += ShaderDataTypeSize(element.Type);
			}
		}

		[[nodiscard]] constexpr uint32_t GetStride()                         const noexcept { return m_Stride;   }
		[[nodiscard]] constexpr std::span<const BufferElement> GetElements() const noexcept { return m_Elements; }

		[[nodiscard]] constexpr std::vector<BufferElement>::iterator begin() noexcept { return m_Elements.begin(); }
		[[nodiscard]] constexpr std::vector<BufferElement>::iterator end()   noexcept { return m_Elements.end();   }

		[[nodiscard]] constexpr std::vector<BufferElement>::const_iterator begin() const noexcept { return m_Elements.begin(); }
		[[nodiscard]] constexpr std::vector<BufferElement>::const_iterator end()   const noexcept { return m_Elements.end();   }

	private: 
		uint32_t m_Stride = 0;
		std::vector<BufferElement> m_Elements;
	};

}
