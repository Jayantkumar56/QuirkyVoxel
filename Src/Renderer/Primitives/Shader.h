

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Utils/NonCopyable.h"

#include <glm/glm.hpp>

#include <string>
#include <string_view>


namespace Mct {

	class Shader : public NonCopyable{
	public:
		Shader(std::string_view vertexShaderSource, std::string_view fragmentShaderSource);
		~Shader() noexcept;

		Shader(Shader&& other)            noexcept;
		Shader& operator=(Shader&& other) noexcept;

		void Bind()   const noexcept;
		void Unbind() const noexcept;

		[[nodiscard]] uint32_t GetId() const noexcept { return m_RendererID; }

		void UploadUniform(std::string_view name, const glm::mat4&  matrix)             const noexcept;
		void UploadUniform(std::string_view name, const glm::vec3&  vec)                const noexcept;
		void UploadUniform(std::string_view name, const glm::vec4&  vec)                const noexcept;
		void UploadUniform(std::string_view name, const glm::ivec2& vec)                const noexcept;
		void UploadUniform(std::string_view name, const int32_t* data, uint32_t count)  const noexcept;
		void UploadUniform(std::string_view name, const float* data, uint32_t count)    const noexcept;

	private:
		int32_t GetUniformLocation(const std::string_view name) const noexcept;

	private:
		uint32_t m_RendererID;
	};

}
