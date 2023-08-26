#include "imnodes_example.h"

#include <imgui/imgui.h>
#include <imnodes/imnodes.h>

#include <vector>

namespace
{
	std::vector<std::pair<int, int>> s_Links = { };
}

namespace ImNodes
{
	void ShowDemoWindow(bool* p_open)
	{
		ImGui::SetNextWindowPos({ 400, 300 }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ 800, 600 }, ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Example Node Editor", p_open, ImGuiWindowFlags_NoSavedSettings))
		{
			ImNodes::BeginNodeEditor();
			ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
			ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkCreationOnSnap);
			{
				ImNodes::BeginNode(1);
				{
					ImNodes::BeginNodeTitleBar();
					ImGui::TextUnformatted("Example Node");
					ImNodes::EndNodeTitleBar();

					ImNodes::BeginInputAttribute(2);
					ImGui::Text("Input");
					ImNodes::EndInputAttribute();

					ImNodes::BeginOutputAttribute(3);
					ImGui::Indent(40);
					ImGui::Text("Output");
					ImNodes::EndOutputAttribute();
				}
				ImNodes::EndNode();

				ImNodes::BeginNode(4);
				{
					ImNodes::BeginNodeTitleBar();
					ImGui::TextUnformatted("Example Node");
					ImNodes::EndNodeTitleBar();

					ImNodes::BeginInputAttribute(5);
					ImGui::Text("Input");
					ImNodes::EndInputAttribute();

					ImNodes::BeginOutputAttribute(6);
					ImGui::Indent(40);
					ImGui::Text("Output");
					ImNodes::EndOutputAttribute();
				}
				ImNodes::EndNode();

				for (int i = 0; i < s_Links.size(); ++i)
				{
					const auto link = s_Links[i];
					ImNodes::Link(i, link.first, link.second);
				}
			}
			ImNodes::PopAttributeFlag();
			ImNodes::PopAttributeFlag();
			ImNodes::MiniMap();
			ImNodes::EndNodeEditor();

			int start_attr, end_attr;
			if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
				s_Links.push_back(std::make_pair(start_attr, end_attr));

			int link_id;
			if (ImNodes::IsLinkDestroyed(&link_id))
				s_Links.erase(s_Links.begin() + link_id);

			if (ImNodes::IsLinkHovered(&link_id) && ImGui::IsKeyPressed(ImGuiKey_Delete))
				s_Links.erase(s_Links.begin() + link_id);
		}
		ImGui::End();
	}

} // namespace example
