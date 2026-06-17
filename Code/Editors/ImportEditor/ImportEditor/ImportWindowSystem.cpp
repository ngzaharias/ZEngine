#include "ImportPCH.h"
#include "ImportEditor/ImportWindowSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/FileDropEvent.h"
#include "ImportEditor/ImportWindowComponent.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

void editor::importer::WindowSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_MenuBar;

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(800, 600.f);

	for (const auto& event : world.Events<const eng::FileDropEvent>())
	{
		auto& windowComponent = !world.HasComponent<editor::importer::WindowComponent>()
			? world.AddComponent<editor::importer::WindowComponent>()
			: world.WriteComponent<editor::importer::WindowComponent>();
		windowComponent.m_Files = event.m_Files;
	}

	for (auto&& view : world.Query<ecs::query::Include<editor::importer::WindowComponent>>())
	{
		auto& window = view.WriteRequired<editor::importer::WindowComponent>();

		bool isWindowOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Asset Importer", &isWindowOpen, s_WindowFlags))
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
			world.RemoveComponent<editor::importer::WindowComponent>();
	}
}