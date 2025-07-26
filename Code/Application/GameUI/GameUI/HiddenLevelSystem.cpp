#include "GameUIPCH.h"
#include "GameUI/HiddenLevelSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Hidden/HiddenObjectComponent.h"
#include "Hidden/HiddenRevealComponent.h"
#include "GameUI/LevelCompleteComponents.h"

void gui::hidden::LevelSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// level complete
	{
		using RevealedQuery = ecs::query
			::Added<const ::hidden::RevealComponent>
			::Include<const ::hidden::ObjectComponent>;
		using RemainingQuery = ecs::query
			::Include<const ::hidden::ObjectComponent>
			::Exclude<const ::hidden::RevealComponent>;
		if (world.HasAny<RevealedQuery>())
		{
			if (!world.HasAny<RemainingQuery>())
			{
				// #todo: load next level
				const ecs::Entity messageEntity = world.CreateEntity();
				world.AddComponent<gui::level_complete::WindowComponent>(messageEntity);
			}
		}
	}
}