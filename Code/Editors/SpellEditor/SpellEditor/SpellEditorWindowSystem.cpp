#include "SpellEditorPCH.h"
#include "SpellEditor/SpellEditorWindowSystem.h"

#include "ECS/EntityView.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameState/GameStateEditorComponent.h"
#include "SpellEditor/SpellEditorHelpers.h"
#include "SpellEditor/SpellEditorGraphComponent.h"
#include "SpellEditor/SpellEditorLinkCreateEvent.h"
#include "SpellEditor/SpellEditorLinkDestroyEvent.h"
#include "SpellEditor/SpellEditorNodeCreateEvent.h"
#include "SpellEditor/SpellEditorWindowComponent.h"
#include "SpellEditor/SpellEditorWindowEvent.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_user.h"
#include "imnodes/imnodes.h"

namespace
{
	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}
}

void editor::spell::WindowSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!world.HasComponent<gamestate::EditorComponent>())
		return;

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(1080, 800.f);

	for (const auto& request : world.Events<editor::spell::WindowEvent>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Spell Editor");

		auto& window = world.AddComponent<editor::spell::WindowComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_Label = ToLabel("Spell Editor", identifier);
	}

	using RemovedQuery = ecs::query
		::Condition<ecs::Alive, ecs::Dead>
		::Removed<editor::spell::WindowComponent>;
	for (auto&& view : world.Query<RemovedQuery>())
	{
		const auto& window = world.ReadComponent<editor::spell::WindowComponent>(view, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (auto&& view : world.Query<ecs::query::Include<editor::spell::WindowComponent, const editor::spell::GraphComponent>>())
	{
		constexpr ImGuiWindowFlags s_WindowFlags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_MenuBar;

		auto& window = view.WriteRequired<editor::spell::WindowComponent>();
		const auto& graphComponent = view.ReadRequired<editor::spell::GraphComponent>();
		const auto& graph = graphComponent.m_Graph;
		const auto& registry = graph.GetRegistry();

		bool isOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(window.m_Label.c_str(), &isOpen, s_WindowFlags))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::EndMenuBar();
			}

			ImNodes::BeginNodeEditor();
			ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
			ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkCreationOnSnap);

			for (auto&& [uuid, node] : graph.GetNodes())
			{
				PushNodeColour(node);
				ImNodes::BeginNode(node.m_GraphId);
				{
					ImNodes::BeginNodeTitleBar();
					ImGui::TextUnformatted(node.m_Label.c_str());
					ImNodes::EndNodeTitleBar();

					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text(node.m_Tooltip.c_str());
						ImGui::EndTooltip();
					}

					ImGui::BeginGroup();
					for (const ngraph::Field& field : node.m_Inputs)
					{
						PushFieldColour(field);
						ImNodes::BeginInputAttribute(field.m_GraphId);
						ImGui::Text(field.m_Label.c_str());
						ImNodes::EndInputAttribute();
						PopFieldColour();
					}
					if (node.m_Inputs.IsEmpty())
					{
						ImGui::Dummy(ImVec2(0, 0));
					}
					ImGui::EndGroup();

					ImGui::SameLine();

					ImGui::BeginGroup();
					for (const ngraph::Field& field : node.m_Outputs)
					{
						PushFieldColour(field);
						ImNodes::BeginOutputAttribute(field.m_GraphId);
						ImGui::Indent(40);
						ImGui::Text(field.m_Label.c_str());
						ImNodes::EndOutputAttribute();
						PopFieldColour();
					}
					if (node.m_Outputs.IsEmpty())
					{
						ImGui::Dummy(ImVec2(0, 0));
					}
					ImGui::EndGroup();
				}
				ImNodes::EndNode();
				PopNodeColour();
			}

			int32 i = 0;
			for (auto&& [uuid, link] : graph.GetLinks())
			{
				PushLinkColour(link);
				ImNodes::Link(i++, link.m_SourceId, link.m_TargetId);
				PopLinkColour();
			}

			ImNodes::PopAttributeFlag();
			ImNodes::PopAttributeFlag();
			ImNodes::MiniMap();
			ImNodes::EndNodeEditor();

			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
				ImGui::OpenPopup("##node");
			if (ImGui::BeginPopupContextItem("##node"))
			{
				for (auto&& [name, node] : registry.GetDefinitions())
				{
					if (ImGui::MenuItem(name.ToChar()))
					{
						auto& event = world.AddEvent<editor::spell::NodeCreateEvent>();
						event.m_Entity = view;
						event.m_Name = name;
					}
				}
				ImGui::EndPopup();
			}

			int32 sourceId, targetId;
			if (ImNodes::IsLinkCreated(&sourceId, &targetId))
			{
				auto& event = world.AddEvent<editor::spell::LinkCreateEvent>();
				event.m_Entity = view;
				event.m_SourceId = sourceId;
				event.m_TargetId = targetId;
			}

			int32 linkId;
			if (ImNodes::IsLinkDestroyed(&linkId))
			{
				auto& event = world.AddEvent<editor::spell::LinkDestroyEvent>();
				event.m_Entity = view;
				event.m_LinkId = linkId;
			}

			if (ImNodes::IsLinkHovered(&linkId) && ImGui::IsKeyPressed(ImGuiKey_Delete))
			{
				auto& event = world.AddEvent<editor::spell::LinkDestroyEvent>();
				event.m_Entity = view;
				event.m_LinkId = linkId;
			}
		}
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(view);
	}
}