#include "GameDebugPCH.h"
#include "GameDebug/InspectorDemoSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameDebug/DebugInspectorDemoRequest.h"
#include "Math/Vector.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"
#include "imgui/InspectorDemo.h"

void dbg::InspectorDemoSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(100.f, 100.f);
	constexpr Vector2f s_DefaultSize = Vector2f(300.f, 200.f);

	for (const auto& request : world.Events<dbg::InspectorDemoRequest>())
		world.AddComponent<dbg::InspectorDemoWindowComponent>(world.CreateEntity());

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const dbg::InspectorDemoWindowComponent>>())
	{
		bool isWindowOpen = true;
		imgui::InspectorDemo(isWindowOpen);

		if (!isWindowOpen)
			world.DestroyEntity(entity);
	}
}