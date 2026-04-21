#include "SpellEditorPCH.h"
#include "SpellEditor/SpellEditorWindowSystem.h"

#include "ECS/EntityView.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameState/GameStateEditorComponent.h"
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

	using NodeView = ecs::EntityView
		::Required<
		editor::spell::WindowComponent>;
	void CreateNode(const NodeView& view, const str::StringView& label)
	{
		auto& window = view.WriteRequired<editor::spell::WindowComponent>();
		{
			const int32 nodeId = window.m_GraphIds++;
			const int32 inputId = window.m_GraphIds++;
			const int32 outputId = window.m_GraphIds++;

			editor::spell::Node& node = window.m_Nodes[nodeId];
			node.m_Label = str::String(label);
			node.m_UUID = str::Guid::Generate();
			node.m_InputId = inputId;
			node.m_OutputId = outputId;

			editor::spell::Param& input = window.m_Param[inputId];
			input.m_NodeId = inputId;

			editor::spell::Param& output = window.m_Param[outputId];
			input.m_NodeId = outputId;
		}
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

	for (auto&& view : world.Query<ecs::query::Include<editor::spell::WindowComponent>>())
	{
		constexpr ImGuiWindowFlags s_WindowFlags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_MenuBar;

		auto& window = view.WriteRequired<editor::spell::WindowComponent>();

		bool isOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(window.m_Label.c_str(), &isOpen, s_WindowFlags))
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Effects"))
				{
					if (ImGui::MenuItem("Damage"))
						CreateNode(view, "Damage");
					if (ImGui::MenuItem("Heal"))
						CreateNode(view, "Heal");
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Emitters"))
				{
					if (ImGui::MenuItem("Explosion"))
						CreateNode(view, "Explosion");
					if (ImGui::MenuItem("Projectile"))
						CreateNode(view, "Projectile");
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			ImNodes::BeginNodeEditor();
			ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
			ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkCreationOnSnap);

			for (auto&& [nodeId, node] : window.m_Nodes)
			{
				ImNodes::BeginNode(nodeId);
				{
					ImNodes::BeginNodeTitleBar();
					ImGui::TextUnformatted(node.m_Label.c_str());
					ImNodes::EndNodeTitleBar();

					ImNodes::BeginInputAttribute(node.m_InputId);
					ImGui::Text("Input");
					ImNodes::EndInputAttribute();

					ImGui::SameLine();

					ImNodes::BeginOutputAttribute(node.m_OutputId);
					ImGui::Text("Output");
					ImNodes::EndOutputAttribute();
				}
				ImNodes::EndNode();
			}

			for (auto&& [linkId, link] : window.m_Links)
			{
				ImNodes::Link(linkId, link.m_SourceId, link.m_TargetId);
			}

			ImNodes::PopAttributeFlag();
			ImNodes::PopAttributeFlag();
			ImNodes::MiniMap();
			ImNodes::EndNodeEditor();

			int32 sourceId, targetId;
			if (ImNodes::IsLinkCreated(&sourceId, &targetId))
			{
				Link& link = window.m_Links[window.m_GraphIds++];
				link.m_SourceId = sourceId;
				link.m_TargetId = targetId;
			}

			int32 linkId;
			if (ImNodes::IsLinkDestroyed(&linkId))
				window.m_Links.Remove(linkId);

			if (ImNodes::IsLinkHovered(&linkId) && ImGui::IsKeyPressed(ImGuiKey_Delete))
				window.m_Links.Remove(linkId);
		}
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(view);
	}
}