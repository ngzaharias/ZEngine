#include "GameDebugPCH.h"
#include "GameDebug/ImGuiDemoSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameDebug/MenuBarComponents.h"
#include "Math/Vector.h"

#include <imgui/imgui.h>

void dbg::ImGuiDemoSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(100.f, 100.f);
	constexpr Vector2f s_DefaultSize = Vector2f(300.f, 200.f);

	const bool hasSingleton = world.HasSingleton<dbg::ImGuiDemoComponent>();

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const dbg::ImGuiDemoRequestComponent>>())
	{
		if (hasSingleton)
			continue;

		world.AddSingleton<dbg::ImGuiDemoComponent>();
	}

	if (hasSingleton)
	{
		bool isWindowOpen = true;
		ImGui::ShowDemoWindow(&isWindowOpen);

		if (!isWindowOpen)
			world.RemoveSingleton<dbg::ImGuiDemoComponent>();
	}
}