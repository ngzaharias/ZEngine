#include "GameDebugPCH.h"
#include "GameDebug/NodeGraphSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameDebug/MenuBarComponents.h"
#include "GameDebug/NodeGraphComponents.h"

#include <imgui/imgui.h>
#include <imnodes/imnodes_example.h>

namespace
{
	constexpr const char* s_Title = "Level: Open";
}

void dbg::NodeGraphSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const dbg::NodeGraphRequestComponent>>())
		world.AddComponent<dbg::NodeGraphComponent>(world.CreateEntity());

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<dbg::NodeGraphComponent>>())
	{
		bool isOpen = true;
		ImNodes::ShowDemoWindow(&isOpen);

		if (!isOpen)
			world.DestroyEntity(entity);
	}
}
