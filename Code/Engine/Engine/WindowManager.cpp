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

#ifdef Z_GLFW
	if (auto* glfwWindow = reinterpret_cast<glfw::Window*>(m_Windows[0]))
		glfwMakeContextCurrent(glfwWindow->GetWindow());

	// #note: glfwMakeContextCurrent must be called before initializing 
	const auto error = glewInit();
	if (error != GLEW_OK)
	{
		const auto* message = glewGetErrorString(error);
		printf("Error: %s\n", message);
		return;
	}

	// #todo: gather available resolutions and refresh rate
	//GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	//int32 modeCount = 0;
	//const GLFWvidmode* modes = glfwGetVideoModes(monitor, &modeCount);
	//for (int32 i = 0; i < modeCount; ++i)
	//{
	//	const GLFWvidmode& mode = modes[i];
	//	Z_LOG(ELog::Debug, "{}x{} ({})", mode.width, mode.height, mode.refreshRate);
	//}
#endif
}

void eng::WindowManager::Shutdown()
{
	for (eng::Window* window : m_Windows)
		delete window;
	m_Windows.RemoveAll();
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
	glfw::Window* glfwWindow = new glfw::Window(config);
	if (!glfwWindow->GetWindow())
	{
		delete glfwWindow;
		return nullptr;
	}

	m_Windows.Append(glfwWindow);
	return glfwWindow;
#endif
	
	return nullptr;
}

bool eng::WindowManager::Destroy(const eng::Window* value)
{
	// #hack: don't remove elements from the list
	for (auto&& [i, window] : enumerate::Forward(m_Windows))
	{
		if (value != window)
			continue;

		delete window;
		window = nullptr;
		return true;
	}
	return false;
}
