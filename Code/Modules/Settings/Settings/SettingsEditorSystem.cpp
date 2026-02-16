#include "Settings/SettingsEditorSystem.h"

#include "Core/Profiler.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Settings/SettingsEditorComponent.h"
#include "Settings/SettingsEditorToggleEvent.h"

void settings::EditorSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

#ifdef Z_EDITOR
	const ecs::Entity entity = world.CreateEntity();
	world.AddComponent<settings::EditorComponent>(entity);
#endif
}

void settings::EditorSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<settings::EditorToggleEvent>())
	{
		if (world.HasAny<ecs::query::Include<settings::EditorComponent>>())
		{
			for (auto&& view : world.Query<ecs::query::Include<settings::EditorComponent>>())
				world.DestroyEntity(view);
		}
		else
		{
			const ecs::Entity entity = world.CreateEntity();
			world.AddComponent<settings::EditorComponent>(entity);
		}
	}
}