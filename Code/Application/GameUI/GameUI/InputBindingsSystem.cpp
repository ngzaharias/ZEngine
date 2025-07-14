#include "GameUIPCH.h"
#include "GameUI/InputBindingsSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/InputManager.h"
#include "GameUI/GameMenuComponents.h"
#include "GameUI/InputComponents.h"

namespace 
{
	const str::Name strInput = str::Name::Create("GUIBinding");
	const str::Name strGameMenu = str::Name::Create("GUIBinding_GameMenu");
}

void gui::input::BindingsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<ecs::query::Added<gui::input::BindingsComponent>>())
	{
		::input::Layer layer;
		layer.m_Priority = eng::EInputPriority::Gameplay;
		layer.m_Bindings.Emplace(strGameMenu, ::input::EKey::Escape);

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInput, layer);
	}

	if (world.HasAny<ecs::query::Removed<gui::input::BindingsComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	const auto& input = world.ReadResource<eng::InputManager>();
	for (const ecs::Entity& bindingEntity : world.Query<ecs::query::Include<const gui::input::BindingsComponent>>())
	{
		const auto& bindings = world.ReadComponent<gui::input::BindingsComponent>(bindingEntity);
		for (const gui::input::ECommand& command : bindings.m_Commands)
		{
			if (command == ECommand::GameMenu && input.IsPressed(strGameMenu))
				world.AddEventComponent<gui::game_menu::OpenRequest>();
		}
	}
}