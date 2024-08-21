#include "EditorPCH.h"
#include "Editor/TableEditor.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameDebug/MenuBarComponents.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_user.h>

namespace
{
	using World = editor::TableEditor::World;

	constexpr ImGuiDockNodeFlags s_DockNodeFlags =
		ImGuiDockNodeFlags_NoCloseButton |
		ImGuiDockNodeFlags_NoWindowMenuButton;
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_MenuBar;

	str::String ToLabel(const char* label, const ecs::Entity& entity)
	{
		return std::format("{}: {}", label, entity.GetIndex());
	}

	void DrawMenuBar(World& world, const ecs::Entity& entity)
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New")) {}

				if (ImGui::MenuItem("Save")) {}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	};

	void DrawBrowser(World& world, const ecs::Entity& entity)
	{
		if (ImGui::CollapsingHeader("Tables: "))
		{
			ImGui::Indent();

			auto& windowComponent = world.WriteComponent<editor::TableWindowComponent>(entity);
			for (auto&& [name, schema] : windowComponent.m_TablesMap)
			{
				if (ImGui::Selectable(name.ToChar()))
					windowComponent.m_TablesOpened.Append(name);
			}

			ImGui::Unindent();
		}
	}

	void DrawInspector(World& world, const ecs::Entity& entity)
	{
		auto& windowComponent = world.WriteComponent<editor::TableWindowComponent>(entity);

		Array<int32> toRemove;
		if (ImGui::BeginTabBar("##tables"))
		{
			for (auto&& [index, tableName] : enumerate::Forward(windowComponent.m_TablesOpened))
			{
				if (!windowComponent.m_TablesMap.Contains(tableName))
					continue;

				ImGui::PushID(index);

				bool isOpen = true;
				if (ImGui::BeginTabItem(tableName.ToChar(), &isOpen))
				{
					const editor::Schema& schema = windowComponent.m_TablesMap.Get(tableName);
					const auto flags =
						ImGuiTableFlags_Resizable |
						ImGuiTableFlags_BordersInnerV |
						ImGuiTableFlags_ScrollY;

					if (ImGui::BeginTable("##table", schema.GetCount(), flags))
					{
						for (const editor::Field& field : schema)
							ImGui::TableSetupColumn(field.m_Name.c_str());
						ImGui::TableHeadersRow();
						ImGui::TableNextRow();

						// 
						ImGui::TableSetColumnIndex(0);

						ImGui::EndTable();
					}

					ImGui::EndTabItem();
				}
				ImGui::PopID();

				if (!isOpen)
					toRemove.Append(index);
			}

			ImGui::EndTabBar();
		}

		for (auto&& [i, index] : enumerate::Reverse(toRemove))
			windowComponent.m_TablesOpened.RemoveOrderedAt(index);
	}
}

void editor::TableEditor::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(1080, 800.f);

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const editor::TableWindowRequestComponent>>())
	{
		const ecs::Entity windowEntity = world.CreateEntity();
		auto& windowComponent = world.AddComponent<editor::TableWindowComponent>(windowEntity);
		windowComponent.m_WindowLabel = ToLabel("Table Editor", windowEntity);
		windowComponent.m_BrowserLabel = ToLabel("Browser", windowEntity);
		windowComponent.m_InspectorLabel = ToLabel("Inspector", windowEntity);

		// Simple
		{
			str::Name tableName = NAME("Simple");
			editor::Schema& schema = windowComponent.m_TablesMap[tableName];
			schema.Emplace("Guid", EType::Guid);
			schema.Emplace("MyNumber", EType::Number);
			schema.Emplace("MyBoolean", EType::Boolean);
		}

		// Complex
		{
			str::Name tableName = NAME("Complex");
			editor::Schema& schema = windowComponent.m_TablesMap[tableName];
			schema.Emplace("Guid", EType::Guid);
			schema.Emplace("Name", EType::String);
			schema.Emplace("DisplayName", EType::String);
			schema.Emplace("Description", EType::String);
			schema.Emplace("MyNumber", EType::Number);
			schema.Emplace("MyBoolean", EType::Boolean);
		}
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<editor::TableWindowComponent>>())
	{
		auto& windowComponent = world.WriteComponent<editor::TableWindowComponent>(windowEntity);

		bool isOpen = true;
		ImGui::SetNextWindowPos({ s_DefaultPos.x, s_DefaultPos.y }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ s_DefaultSize.x, s_DefaultSize.y }, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(windowComponent.m_WindowLabel.c_str(), &isOpen, s_WindowFlags))
		{
			DrawMenuBar(world, windowEntity);

			const ImGuiID dockspaceId = ImGui::GetID(windowComponent.m_WindowLabel.c_str());
			if (!ImGui::DockBuilderGetNode(dockspaceId))
			{
				ImGui::DockBuilderRemoveNode(dockspaceId);
				ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetWindowSize());

				ImGuiID browserId, inspectorId;
				ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.3f, &browserId, &inspectorId);
				ImGui::DockBuilderDockWindow(windowComponent.m_BrowserLabel.c_str(), browserId);
				ImGui::DockBuilderDockWindow(windowComponent.m_InspectorLabel.c_str(), inspectorId);
				ImGui::DockBuilderFinish(dockspaceId);
			}
			ImGui::DockSpace(dockspaceId, ImVec2(0, 0), s_DockNodeFlags);
		}
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_BrowserLabel.c_str()))
			DrawBrowser(world, windowEntity);
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_InspectorLabel.c_str()))
			DrawInspector(world, windowEntity);
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(windowEntity);
	}
}