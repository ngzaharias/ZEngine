#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorInspector.h"

editor::entity::Inspector::~Inspector()
{
	// cleanup any commands that weren't moved
	for (Command* command : m_Commands)
		delete command;
	m_Commands.RemoveAll();
}

bool editor::entity::Inspector::Begin(const char* label, const str::Guid& entityUUID)
{
	const ImGuiTableFlags flags =
		ImGuiTableFlags_SizingFixedFit |
		ImGuiTableFlags_BordersInnerV |
		ImGuiTableFlags_Resizable;

	// 1 column for label
	// 1 column for data
	// 1 column for extras
	m_EntityUUID = entityUUID;
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

void editor::entity::Inspector::End()
{
	ImGui::EndTable();
}

bool editor::entity::Inspector::HasCommands() const
{
	return !m_Commands.IsEmpty();
}

auto editor::entity::Inspector::GetCommands() const -> const Array<Command*>
{
	return m_Commands;
}

auto editor::entity::Inspector::MoveCommands() -> Array<Command*>
{
	Array<Command*> command = m_Commands;
	m_Commands.RemoveAll();
	return command;
}