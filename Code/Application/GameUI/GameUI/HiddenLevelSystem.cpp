#include "GameUIPCH.h"
#include "GameUI/HiddenLevelSystem.h"

#include "ClientHidden/HiddenObjectComponent.h"
#include "ClientHidden/HiddenRevealComponent.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameUI/LevelCompleteWindowComponent.h"

void gui::hidden::LevelSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// level complete
	{
		using RevealedQuery = ecs::query
			::Added<const client::hidden::RevealComponent>
			::Include<const client::hidden::ObjectComponent>;
		using RemainingQuery = ecs::query
			::Include<const client::hidden::ObjectComponent>
			::Exclude<const client::hidden::RevealComponent>;
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