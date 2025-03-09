#include "EnginePCH.h"
#include "Engine/WindowSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/GLFW/Window.h"
#include "Engine/SettingsComponents.h"
#include "Engine/Window.h"
#include "Engine/WindowComponents.h"

#include <GLFW/glfw3.h>

void eng::WindowSystem::Initialise(World& world)
{
}

void eng::WindowSystem::Shutdown(World& world)
{
}

void eng::WindowSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<ecs::query::Updated<eng::settings::GraphicsComponent>>())
	{
		const auto& settings = world.ReadSingleton<eng::settings::GraphicsComponent>();
		const auto& resource = world.ReadResource<eng::Window>();
		if (const glfw::Window* glfwWindow = dynamic_cast<const glfw::Window*>(&resource))
		{
			{
				GLFWmonitor* monitor = glfwGetPrimaryMonitor();

				int32 modeCount = 0;
				const GLFWvidmode* modes = glfwGetVideoModes(monitor, &modeCount);
				for (int32 i = 0; i < modeCount; ++i)
				{
					const GLFWvidmode& mode = modes[i];
					Z_LOG(ELog::Debug, "{}x{} ({})", mode.width, mode.height, mode.refreshRate);
				}
			}

			auto* window = glfwWindow->GetWindow();
			switch (settings.m_Mode)
			{
			case eng::settings::EMode::Fullscreen:
			{
				GLFWmonitor* monitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);

				// #bug: setting the monitor with a resolution that doesn't 
				// match causes the window to become unresponsive
				glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			} break;

			case eng::settings::EMode::Windowed:
			{
				GLFWmonitor* monitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);
				const Vector2u& resolution = settings.m_Resolution;
				const Vector2i position = Vector2i(
					mode->width / 2 - resolution.x / 2,
					mode->height / 2 - resolution.y / 2);

				glfwSetWindowMonitor(window, nullptr, position.x, position.y, resolution.x, resolution.y, mode->refreshRate);
			} break;

			case eng::settings::EMode::Borderless:
			{
				GLFWmonitor* monitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);

				glfwSetWindowMonitor(window, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
			} break;
			}
		}
	}
}