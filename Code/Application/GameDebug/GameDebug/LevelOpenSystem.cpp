#include "GameDebugPCH.h"
#include "GameDebug/LevelOpenSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/InputManager.h"
#include "Engine/LevelComponents.h"
#include "GameDebug/LevelOpenComponents.h"
#include "Input/Key.h"
#include "Math/Math.h"
#include "Math/Vector.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

namespace
{
	constexpr const char* s_Title = "Level: Open";

	constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
	constexpr float s_DesiredWidth = 512.f;

	constexpr ImGuiTableFlags s_TableFlags =
		ImGuiTableFlags_ContextMenuInBody |
		ImGuiTableFlags_Resizable |
		ImGuiTableFlags_Sortable;

	const str::Name strInput = str::Name::Create("LevelOpen");
	const str::Name strClose = str::Name::Create("LevelOpen_Close");
}

void dbg::level::OpenSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const bool hasWindow = world.HasAny<ecs::query::Include<dbg::level::WindowComponent>>();
	if (!hasWindow && world.HasAny<dbg::level::OpenRequest>())
	{
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, s_Title);
		world.AddComponent<dbg::level::WindowComponent>(windowEntity);

		ImGui::OpenPopup(s_Title);

		{
			input::Layer layer;
			layer.m_Priority = eng::EInputPriority::Debug + 1;
			layer.m_Bindings.Emplace(strClose, input::EKey::Escape);

			auto& input = world.WriteResource<eng::InputManager>();
			input.AppendLayer(strInput, layer);
		}
	}

	if (world.HasAny<ecs::query::Removed<dbg::level::WindowComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<dbg::level::WindowComponent>>())
	{
		constexpr ImGuiWindowFlags s_WindowFlags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoDocking;

		bool isWindowOpen = true;
		ImGui::SetNextWindowSize({ s_DefaultSize.x, s_DefaultSize.y }, ImGuiCond_FirstUseEver);
		if (ImGui::BeginPopupModal(s_Title, &isWindowOpen, s_WindowFlags))
		{
			const ImVec2 region = ImGui::GetContentRegionAvail();
			const int32 count = math::Max(1, math::Round<int32>(region.x / s_DesiredWidth));

			if (ImGui::BeginTable("##table", count, s_TableFlags))
			{
				ImGui::TableNextRow();
				ImGui::PushItemWidth(-1);

				const float width = ImGui::GetContentRegionAvail().x / count;
				const float height = 64.f;

				const auto& directoryComponent = world.WriteSingleton<eng::level::DirectorySingleton>();
				for (auto&& [name, path] : directoryComponent.m_Levels)
				{
					ImGui::TableNextColumn();

					if (ImGui::Button(name.ToChar(), { width, 0 }))
					{
						world.AddEvent<eng::level::LoadRequest>(name);
						ImGui::CloseCurrentPopup();
					}

					// #todo: fetch image from folder
					imgui::Image(0, Vector2f(width, height));
				}
				ImGui::EndTable();
			}
			ImGui::EndPopup();
		}

		const auto& input = world.ReadResource<eng::InputManager>();
		if (!isWindowOpen || input.IsPressed(strClose))
			world.DestroyEntity(entity);
	}
}
