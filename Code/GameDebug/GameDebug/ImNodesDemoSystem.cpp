#include "GameDebugPCH.h"
#include "GameDebug/ImNodesDemoSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameDebug/MenuBarComponents.h"
#include "Math/Vector.h"

#include <imnodes/imnodes_example.h>

void dbg::ImNodesDemoSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(100.f, 100.f);
	constexpr Vector2f s_DefaultSize = Vector2f(300.f, 200.f);

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const dbg::ImNodesDemoRequestComponent>>())
		world.AddComponent<dbg::ImNodesDemoWindowComponent>(world.CreateEntity());

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const dbg::ImNodesDemoWindowComponent>>())
	{
		bool isWindowOpen = true;
		ImNodes::ShowDemoWindow(&isWindowOpen);

		if (!isWindowOpen)
			world.DestroyEntity(entity);
	}
}