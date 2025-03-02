#include "GameUIPCH.h"
#include "GameUI/InputBindingsSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/InputComponent.h"
#include "GameUI/GameMenuComponents.h"
#include "GameUI/InputComponents.h"

namespace 
{
	bool IsInputPressed(const eng::InputComponent& input, const gui::input::Input& value)
	{
		return std::visit([&](auto&& data) { return input.IsKeyPressed(data); }, value);
	}
}

void gui::input::BindingsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();
	for (const ecs::Entity& inputEntity : world.Query<ecs::query::Updated<const eng::InputComponent>>())
	{
		const auto& input = world.ReadComponent<eng::InputComponent>(inputEntity);
		for (const ecs::Entity& bindingEntity : world.Query<ecs::query::Include<const gui::input::BindingsComponent>>())
		{
			const auto& bindings = world.ReadComponent<gui::input::BindingsComponent>(bindingEntity);
			for (const gui::input::Binding& binding : bindings.m_Bindings)
			{
				if (IsInputPressed(input, binding.m_Input))
				{
					switch (binding.m_Command)
					{
					case ECommand::ToggleGameMenu:
						world.AddEventComponent<gui::game_menu::OpenRequestComponent>();
						break;
					}
				}
			}
		}
	}
}