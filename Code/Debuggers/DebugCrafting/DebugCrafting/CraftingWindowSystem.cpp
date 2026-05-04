#include "DebugCraftingPCH.h"
#include "DebugCrafting/CraftingWindowSystem.h"

#include "Core/String.h"
#include "DebugCrafting/CraftingWindowComponent.h"
#include "DebugCrafting/CraftingWindowEvent.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Math/Vector.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

namespace
{
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoCollapse;

	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}
}

void debug::crafting::WindowSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(100.f, 350.f);
	constexpr Vector2f s_DefaultSize = Vector2f(300.f, 200.f);

	for (const auto& request : world.Events<debug::crafting::WindowEvent>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();

		auto& window = world.AddComponent<debug::crafting::WindowComponent>(windowEntity);
		window.m_Label = ToLabel("Crafting Debugger", identifier);
		window.m_Identifier = identifier;
	}

	using RemovedQuery = ecs::query
		::Condition<ecs::Alive, ecs::Dead>
		::Removed<debug::crafting::WindowComponent>;
	for (auto&& view : world.Query<RemovedQuery>())
	{
		const auto& window = world.ReadComponent<debug::crafting::WindowComponent>(view, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (auto&& windowView : world.Query<ecs::query::Include<debug::crafting::WindowComponent>>())
	{
		auto& window = windowView.WriteRequired<debug::crafting::WindowComponent>();

		bool isWindowOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(window.m_Label.c_str(), &isWindowOpen, s_WindowFlags))
		{
			ImGui::End();

		}

		if (!isWindowOpen)
			world.DestroyEntity(windowView);
	}
}