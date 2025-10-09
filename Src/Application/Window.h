

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <optional>


class Window {
public:
	static std::optional<Window> Create();

private:
	Window() noexcept;
};
