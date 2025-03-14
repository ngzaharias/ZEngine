#include "GameDebugPCH.h"
#include "GameDebug/FrameBufferSystem.h"

#include "Core/String.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/FrameBufferComponent.h"
#include "Math/Vector.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

void dbg::FrameBufferSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(100.f, 350.f);
	constexpr Vector2f s_DefaultSize = Vector2f(300.f, 200.f);

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const dbg::BufferWindowRequestComponent>>())
		world.AddComponent<dbg::BufferWindowComponent>(world.CreateEntity());

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<dbg::BufferWindowComponent>>())
	{
		const str::String label = "Render: Frame Buffer##" + std::to_string(entity.GetIndex());

		bool isOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(label.c_str(), &isOpen))
		{
			const auto& component = world.ReadSingleton<eng::FrameBufferComponent>();

			const ImVec2 regionSize = ImGui::GetContentRegionAvail();
			imgui::Image(component.m_ShadowTexture, Vector2f(regionSize.x, regionSize.y));
		}
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(entity);
	}
}