#include "EnginePCH.h"
#include "Engine/WindowManager.h"

#include "Core/Algorithms.h"
#include "Core/Path.h"
#include "Engine/GLFW/Window.h"
#include "Engine/WindowConfig.h"
#include "Engine/WindowModeEnum.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

namespace
{
	static void glfw_error_callback(int error, const char* description)
	{
		fprintf(stderr, "Window: Error - %d: %s.\n", error, description);
	}
}

void eng::WindowManager::Initialise()
{
	Z_LOG(ELog::Debug, "Window: Initialise - GLFW.");

	glfwSetErrorCallback(glfw_error_callback);

	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	eng::WindowConfig config;
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		config.m_Resolution = Vector2u(mode->width, mode->height);
		config.m_Position = config.m_Resolution / 2;
		config.m_RefreshRate = mode->refreshRate;
	}

	Create(config);
	if (m_Windows.IsEmpty())
		return;

#ifdef Z_GLFW
	if (auto* window = reinterpret_cast<glfw::Window*>(m_Windows[0]))
	{
		auto* glfwWindow = window->GetWindow();
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetWindowTitle(glfwWindow, "With My Little Eye");
		glfwMakeContextCurrent(glfwWindow);

		int channels;
		GLFWimage icon;
		const str::Path filepath = str::Path(str::EPath::WorkingDir, "ClientIcon.png");
		if (icon.pixels = stbi_load(filepath.ToChar(), &icon.width, &icon.height, &channels, STBI_rgb_alpha))
		{
			glfwSetWindowIcon(glfwWindow, 1, &icon);
			stbi_image_free(icon.pixels);
		}
	}

	glfwSwapInterval(config.m_IsVSyncEnabled);

	// #note: glfwMakeContextCurrent must be called before initializing 
	const auto error = glewInit();
	if (error != GLEW_OK)
	{
		const auto* message = glewGetErrorString(error);
		printf("Error: %s\n", message);
		return;
	}

	Set<int32> refreshRates = {};
	Set<Vector2u> resolutions = {};

	int monitorCount = 0;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
	for (int32 monitorIndex = 0; monitorIndex < monitorCount; ++monitorIndex)
	{
		GLFWmonitor* glfwMonitor = monitors[monitorIndex];

		int32 modeCount = 0;
		const GLFWvidmode* modes = glfwGetVideoModes(glfwMonitor, &modeCount);
		for (int32 i = 0; i < modeCount; ++i)
		{
			const GLFWvidmode& mode = modes[i];
			resolutions.Add(Vector2u(mode.width, mode.height));
			refreshRates.Add(mode.refreshRate);
		}

		int xpos, ypos;
		glfwGetMonitorPos(glfwMonitor, &xpos, &ypos);
		const GLFWvidmode* mode = glfwGetVideoMode(glfwMonitor);
		eng::Monitor& monitor = m_Monitors.Emplace();
		monitor.m_Resolution = Vector2u(mode->width, mode->height);
		monitor.m_Position = Vector2u(xpos, ypos);
		monitor.m_RefreshRate = mode->refreshRate;
	}

	m_WindowModes.Append(eng::EWindowMode::Borderless);
	m_WindowModes.Append(eng::EWindowMode::Fullscreen);
	m_WindowModes.Append(eng::EWindowMode::Windowed);

	for (const Vector2u& value : resolutions)
		m_Resolutions.Append(value);
	for (const int32 value : refreshRates)
		m_RefreshRates.Append(value);

#endif
}

void eng::WindowManager::Shutdown()
{
	for (eng::Window* window : m_Windows)
		delete window;
	m_Windows.RemoveAll();

	Z_LOG(ELog::Debug, "Window: Shutdown - GLFW.");
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

const eng::Monitor* eng::WindowManager::GetMonitor(const int32 index) const
{
	if (index < m_Monitors.GetCount())
		return &m_Monitors[index];
	return nullptr;
}

const Array<eng::Monitor>& eng::WindowManager::GetMonitors() const
{
	return m_Monitors;
}

eng::Window* eng::WindowManager::GetWindow(const int32 index) 
{ 
	if (index < m_Windows.GetCount())
		return m_Windows[index]; 
	return nullptr;
}

const eng::Window* eng::WindowManager::GetWindow(const int32 index) const 
{ 
	if (index < m_Windows.GetCount())
		return m_Windows[index];
	return nullptr;
}

const Array<eng::Window*>& eng::WindowManager::GetWindows() const
{
	return m_Windows;
}