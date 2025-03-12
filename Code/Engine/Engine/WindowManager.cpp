#include "EnginePCH.h"
#include "Engine/WindowManager.h"

#include "Core/Algorithms.h"
#include "Engine/GLFW/Window.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	static void glfw_error_callback(int error, const char* description)
	{
		fprintf(stderr, "Glfw Error %d: %s\n", error, description);
	}
}

void eng::WindowManager::Initialise(const eng::WindowConfig& config)
{
	glfwSetErrorCallback(glfw_error_callback);

	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Create(config);
	if (m_Windows.IsEmpty())
		return;

	if (auto* window = reinterpret_cast<glfw::Window*>(m_Windows[0]))
		glfwMakeContextCurrent(window->GetWindow());

	// #note: glfwMakeContextCurrent must be called before initialising glew 
	const auto error = glewInit();
	if (error != GLEW_OK)
	{
		const auto* message = glewGetErrorString(error);
		printf("Error: %s\n", message);
		return;
	}
}

void eng::WindowManager::Shutdown()
{
	Destroy(GetPrimary());
}

void eng::WindowManager::PreUpdate(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (eng::Window* window : m_Windows)
		window->PreUpdate(gameTime);
}

void eng::WindowManager::PostUpdate(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (eng::Window* window : m_Windows)
		window->PostUpdate(gameTime);
}

auto eng::WindowManager::Create(const WindowConfig& config) -> const eng::Window*
{
#ifdef Z_GLFW
	glfw::Window* window = new glfw::Window(config);
	if (window->GetWindow())
	{
		m_Windows.Append(window);
		return window;
	}
	else
	{
		delete window;
	}
#endif
	
	return nullptr;
}

bool eng::WindowManager::Destroy(const eng::Window* value)
{
	for (const auto& [i, window] : enumerate::Forward(m_Windows))
	{
		if (value != window)
			continue;

		m_Windows.RemoveAt(i);
		return true;
	}
	return false;
}
