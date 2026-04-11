
template<typename Value>
void imgui::Inspector::Read(const Value& value)
{
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(1);
	imgui::ReadMember(value);
}

template<typename Value>
void imgui::Inspector::Read(const char* label, const Value& value)
{
	ImGui::TableNextRow();

	RaiiID id(label);
	if (imgui::ReadHeader(label, value))
	{
		RaiiIndent indent(0);
		ImGui::TableSetColumnIndex(1);
		imgui::ReadMember(value);
	}
}

template<typename Value>
inline bool imgui::Inspector::Write(Value& value)
{
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(1);
	return imgui::WriteMember(value);
}

template<typename Value>
inline bool imgui::Inspector::Write(const char* label, Value& value)
{
	ImGui::TableNextRow();

	RaiiID id(label);
	bool modified = false;
	ImGui::TableSetColumnIndex(1);
	modified |= imgui::WriteDetails(value);
	if (imgui::WriteHeader(label, value))
	{
		RaiiIndent indent(0);
		ImGui::TableSetColumnIndex(1);
		modified |= imgui::WriteMember(value);
	}
	return modified;
}