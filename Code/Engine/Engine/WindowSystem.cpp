#include "EnginePCH.h"
#include "Engine/WindowSystem.h"

#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SettingsComponents.h"
#include "Engine/GLFW/Window.h"
#include "Engine/WindowManager.h"
#include "Engine/Visitor.h"

#include <GLFW/glfw3.h>

namespace
{
	const str::StringView strFilename = "GraphicsSettings.toml";
}

void eng::WindowSystem::Initialise(World& world)
{
	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);
	auto& settings = world.WriteSingleton<eng::settings::GraphicsComponent>();

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(settings);

	auto& manager = world.WriteResource<eng::WindowManager>();
	eng::Window* window = manager.GetWindow(0);
	window->SetMode(settings.m_Mode);
	window->SetResolution(settings.m_Resolution);
	window->Refresh();
}

void eng::WindowSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<ecs::query::Updated<eng::settings::GraphicsComponent>>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);
		const auto& settings = world.ReadSingleton<eng::settings::GraphicsComponent>();

		auto& manager = world.WriteResource<eng::WindowManager>();
		eng::Window* window = manager.GetWindow(0);
		window->SetMode(settings.m_Mode);
		window->SetResolution(settings.m_Resolution);
		window->Refresh();

		eng::Visitor visitor;
		visitor.Write(settings);
		visitor.SaveToFile(filepath);
	}
}