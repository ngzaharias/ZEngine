#include "AssetPCH.h"
#include "AssetEditor/AssetImporterSystem.h"

#include "AssetEditor/AssetImporterComponent.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/FileDropEvent.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

void editor::assets::ImporterSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_MenuBar;

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(800, 600.f);

	for (const auto& event : world.Events<const eng::FileDropEvent>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity entity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(entity, "Asset Importer");

		auto& window = world.AddComponent<editor::assets::ImporterComponent>(entity);
		window.m_Identifier = identifier;
		window.m_Label = std::format("Asset Importer: {}", identifier);
		window.m_Files = event.m_Files;
	}

	using RemovedQuery = ecs::query
		::Condition<ecs::Alive, ecs::Dead>
		::Removed<editor::assets::ImporterComponent>;
	for (auto&& view : world.Query<RemovedQuery>())
	{
		const auto& window = world.ReadComponent<editor::assets::ImporterComponent>(view, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (auto&& view : world.Query<ecs::query::Include<editor::assets::ImporterComponent>>())
	{
		auto& window = view.WriteRequired<editor::assets::ImporterComponent>();

		bool isWindowOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(window.m_Label.c_str(), &isWindowOpen, s_WindowFlags))
		{
			const str::Path filepath = window.m_Files.GetLast();
			ImGui::Selectable(filepath.ToChar());

			if (ImGui::Button("Import"))
				window.m_Files.RemoveLast();
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
				window.m_Files.RemoveLast();
		}
		ImGui::End();

		if (!isWindowOpen || window.m_Files.IsEmpty())
			world.DestroyEntity(view);
	}
}