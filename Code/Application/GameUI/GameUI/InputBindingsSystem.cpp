#include "GameUIPCH.h"
#include "GameUI/InputBindingsSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/InputManager.h"
#include "GameUI/GameMenuOpenEvent.h"
#include "GameUI/InputBindingsTemplate.h"

namespace 
{
	const str::Name strInput = str::Name::Create("GUIBinding");
	const str::Name strGameMenu = str::Name::Create("GUIBinding_GameMenu");
}

void gui::input::BindingsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<ecs::query::Added<gui::input::BindingsTemplate>>())
	{
		::input::Layer layer;
		layer.m_Priority = eng::EInputPriority::Gameplay;
		layer.m_Bindings.Emplace(strGameMenu, ::input::EKey::Escape);

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInput, layer);
	}

	using RemovedQuery = ecs::query
		::Condition<ecs::Alive, ecs::Dead>
		::Removed<gui::input::BindingsTemplate>;
	if (world.HasAny<RemovedQuery>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	const auto& input = world.ReadResource<eng::InputManager>();
	for (auto&& view : world.Query<ecs::query::Include<const gui::input::BindingsTemplate>>())
	{
		const auto& bindings = view.ReadRequired<gui::input::BindingsTemplate>();
		for (const gui::input::ECommand& command : bindings.m_Commands)
		{
			if (command == ECommand::GameMenu && input.IsPressed(strGameMenu))
				world.AddEvent<gui::game_menu::OpenEvent>();
		}
	}
}