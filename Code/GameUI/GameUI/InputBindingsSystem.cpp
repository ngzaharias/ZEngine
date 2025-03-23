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
	const str::Guid strInputGuid = str::Guid::Generate();
	const str::Name strGameMenu = str::Name::Create("GUIBinding_GameMenu");
}

void gui::input::BindingsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& input = world.ReadResource<eng::InputManager>();
	for (const ecs::Entity& bindingEntity : world.Query<ecs::query::Include<const gui::input::BindingsComponent>>())
	{
		const auto& bindings = world.ReadComponent<gui::input::BindingsComponent>(bindingEntity);
		for (const gui::input::ECommand& command : bindings.m_Commands)
		{
			if (command == ECommand::GameMenu && input.IsPressed(strGameMenu))
				world.AddEventComponent<gui::game_menu::OpenRequestComponent>();
		}
	}
}