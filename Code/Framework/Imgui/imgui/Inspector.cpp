#include "imgui/Inspector.h"

#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "Core/String.h"
#include "Core/StringView.h"
#include "Core/Types.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"

bool imgui::Inspector::Begin(const char* label)
{
	const ImGuiTableFlags flags =
		ImGuiTableFlags_SizingFixedFit |
		ImGuiTableFlags_BordersInnerV |
		ImGuiTableFlags_Resizable;

	// 1 column for the label
	// 1 column for the data
	// 1 column for extra data
	if (ImGui::BeginTable(label, 3, flags))
	{
		constexpr ImGuiTableColumnFlags s_Column0 = ImGuiTableColumnFlags_WidthFixed;
		constexpr ImGuiTableColumnFlags s_Column1 =
			ImGuiTableColumnFlags_IndentDisable |
			ImGuiTableColumnFlags_WidthStretch;
		constexpr ImGuiTableColumnFlags s_Column2 =
			ImGuiTableColumnFlags_IndentDisable |
			ImGuiTableColumnFlags_NoResize |
			ImGuiTableColumnFlags_WidthFixed;

		ImGui::TableSetupColumn("Name ", s_Column0);
		ImGui::TableSetupColumn("Value", s_Column1);
		ImGui::TableSetupColumn("##Extra", s_Column2);
		ImGui::TableHeadersRow();
		return true;
	}

	return false;
}

void imgui::Inspector::End()
{
	ImGui::EndTable();
}