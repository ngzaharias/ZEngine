#include "GameDebugPCH.h"
#include "GameDebug/ImGuiDemoSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameDebug/DebugImGuiDemoRequest.h"
#include "GameDebug/ImGuiDemoWindowComponent.h"
#include "Math/Vector.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

void debug::ImGuiDemoSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(100.f, 100.f);
	constexpr Vector2f s_DefaultSize = Vector2f(300.f, 200.f);

	for (const auto& request : world.Events<debug::ImGuiDemoRequest>())
		world.AddComponent<debug::ImGuiDemoWindowComponent>(world.CreateEntity());

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const debug::ImGuiDemoWindowComponent>>())
	{
		bool isWindowOpen = true;
		ImGui::ShowDemoWindow(&isWindowOpen);

		if (!isWindowOpen)
			world.DestroyEntity(entity);
	}
}