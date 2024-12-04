#include "GameDebugPCH.h"
#include "GameDebug/LevelOpenSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelComponents.h"
#include "Math/Math.h"
#include "Math/Vector.h"

#include <imgui/imgui.h>
#include <imgui/imgui_user.h>

namespace
{
	constexpr const char* s_Title = "Level: Open";
}

void dbg::level::OpenSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
	constexpr float s_DesiredWidth = 512.f;
	constexpr ImGuiTableFlags s_TableFlags =
		ImGuiTableFlags_ContextMenuInBody |
		ImGuiTableFlags_Resizable |
		ImGuiTableFlags_Sortable;
	constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
	constexpr ImGuiWindowFlags s_WindowFlags = 
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoDocking;

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const dbg::level::OpenRequestComponent>>())
		ImGui::OpenPopup(s_Title, s_PopupFlags);

	bool isOpen = true;
	ImGui::SetNextWindowSize({ s_DefaultSize.x, s_DefaultSize.y }, ImGuiCond_FirstUseEver);
	if (ImGui::BeginPopupModal(s_Title, &isOpen, s_WindowFlags))
	{
		const ImVec2 region = ImGui::GetContentRegionAvail();
		const int32 count = math::Max(1, math::Round<int32>(region.x / s_DesiredWidth));

		if (ImGui::BeginTable("##table", count, s_TableFlags))
		{
			ImGui::TableNextRow();
			ImGui::PushItemWidth(-1);

			const float width = ImGui::GetContentRegionAvail().x / count;
			const float height = 64.f;

			const auto& directoryComponent = world.WriteSingleton<eng::level::DirectoryComponent>();
			for (auto&& [name, path] : directoryComponent.m_Levels)
			{
				ImGui::TableNextColumn();

				if (ImGui::Button(name.ToChar(), { width, 0 }))
				{
					world.AddEventComponent<eng::level::LoadRequestComponent>(name);
					ImGui::CloseCurrentPopup();
				}

				// #todo: fetch image from folder
				imgui::Image(0, Vector2f(width, height));
			}
			ImGui::EndTable();
		}
		ImGui::EndPopup();
	}
}
