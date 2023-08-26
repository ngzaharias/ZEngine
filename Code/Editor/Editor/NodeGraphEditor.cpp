#include "EditorPCH.h"
#include "NodeGraphEditor.h"

#include <imgui/imgui.h>
#include <imgui-nodes/imnodes.h>

void editor::NodeGraphEditor::Initialise()
{
	ImNodes::SetNodeGridSpacePos(1, ImVec2(200.0f, 200.0f));
	ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;

	CreateNode("First Node");
	CreateNode("Second Node");
	CreateNode("Third Node");
}

void editor::NodeGraphEditor::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!m_IsVisible)
		return;

	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_MenuBar;

	if (ImGui::Begin("Node Editor", &m_IsVisible, flags))
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::EndMenuBar();
		}

		ImNodes::BeginNodeEditor();
		ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
		ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkCreationOnSnap);

		for (const auto& node : m_Nodes)
		{
			ImNodes::BeginNode(node.m_Handle);
			{
				ImNodes::BeginNodeTitleBar();
				ImGui::TextUnformatted(node.m_Label.c_str());
				ImNodes::EndNodeTitleBar();

				for (const auto& link : node.m_Inputs)
				{
					ImNodes::BeginInputAttribute(link);
					ImGui::Text("Input");
					ImNodes::EndInputAttribute();
				}

				for (const auto& link : node.m_Outputs)
				{
					ImNodes::BeginInputAttribute(link);
					ImGui::Indent(40);
					ImGui::Text("Output");
					ImNodes::EndInputAttribute();
				}
			}
			ImNodes::EndNode();
		}

		for (auto&& [i, link] : enumerate::Forward(m_Links))
			ImNodes::Link(i, link.first, link.second);

		ImNodes::PopAttributeFlag();
		ImNodes::PopAttributeFlag();
		ImNodes::MiniMap();
		ImNodes::EndNodeEditor();

		int start_attr, end_attr;
		if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
			m_Links.Append(std::make_pair(start_attr, end_attr));

		int link_id;
		if (ImNodes::IsLinkDestroyed(&link_id))
			m_Links.RemoveOrderedAt(link_id);

		if (ImNodes::IsLinkHovered(&link_id) && ImGui::IsKeyPressed(ImGuiKey_Delete))
			m_Links.RemoveOrderedAt(link_id);
	}
	ImGui::End();
}

void editor::NodeGraphEditor::CreateNode(const str::StringView& label)
{
	node::Node node;
	node.m_Label = label;
	node.m_Handle = m_HandlesFree.IsEmpty()
		? m_HandleLast++
		: m_HandlesFree.Pop();

	m_Nodes.Append(std::move(node));
}

void editor::NodeGraphEditor::DestroyNode(const node::Node& node)
{
	m_HandlesFree.Append(node.m_Handle);
}
