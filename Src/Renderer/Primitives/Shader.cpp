

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Common/Assert.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


namespace Mct {

	static GLuint CompileShader(const GLenum glType, const std::string_view source) noexcept;

	Shader::Shader(std::string_view vertexShaderSource, std::string_view fragmentShaderSource) {
		GLuint vertexShader   = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererId = glCreateProgram();
		GLuint program = m_RendererId;

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			std::cerr << "Shader link failure with error " << infoLog.data();
			MCT_ASSERT(false);
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	Shader::~Shader() noexcept {
		glDeleteProgram(m_RendererId);
	}

	Shader::Shader(Shader&& other) noexcept :
			m_RendererId(other.m_RendererId)
	{
		other.m_RendererId = 0;
	}

	Shader& Shader::operator=(Shader&& other) noexcept {
		if (this != &other) {
			if (m_RendererId > 0) {
				glDeleteProgram(m_RendererId);
			}

			m_RendererId       = other.m_RendererId;
			other.m_RendererId = 0;
		}

		return *this;
	}

	void Shader::Bind() const noexcept {
		glUseProgram(m_RendererId);
	}

	void Shader::Unbind() const noexcept {
		glUseProgram(0);
	}

	void Shader::UploadUniform(const std::string_view name, const glm::mat4& matrix) const noexcept {
		GLint location = glGetUniformLocation(m_RendererId, name.data());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::UploadUniform(const std::string_view name, const glm::vec3& vec) const noexcept {
		GLint location = glGetUniformLocation(m_RendererId, name.data());
		glUniform3fv(location, 1, glm::value_ptr(vec));
	}

	void Shader::UploadUniform(const std::string_view name, const glm::vec4& vec) const noexcept {
		GLint location = glGetUniformLocation(m_RendererId, name.data());
		glUniform4fv(location, 1, glm::value_ptr(vec));
	}

	void Shader::UploadUniform(const std::string_view name, const int32_t* data, uint32_t count) const noexcept {
		GLint location = glGetUniformLocation(m_RendererId, name.data());
		glUniform1iv(location, count, data);
	}

	void Shader::UploadUniform(const std::string_view name, const float* data, uint32_t count) const noexcept {
		GLint location = glGetUniformLocation(m_RendererId, name.data());
		glUniform1fv(location, count, data);
	}


	static GLuint CompileShader(const GLenum glType, const std::string_view shaderSource) noexcept {
		GLuint shader = glCreateShader(glType);

		const GLchar* source = shaderSource.data();
		const GLint   size   = static_cast<int>(shaderSource.size());

		glShaderSource(shader, 1, &source, &size);
		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(shader);

			MCT_ASSERT("Shader compilation failure with error {0}" && infoLog.data());
			return 0;
		}

		return shader;
	}

}
