
template<typename Value>
void editor::entity::Inspector::Visit(const Value& value)
{
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(1);
	VisitCustom(value);
}

template<typename Value>
void editor::entity::Inspector::Visit(const char* label, const Value& value)
{
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);

	ImGui::PushID(label);
	if (ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Indent();
		ImGui::TableSetColumnIndex(1);
		VisitCustom(value);
		ImGui::Unindent();
	}
	ImGui::PopID();
}

template <typename TComponent, typename TValue>
void editor::entity::Inspector::PushCommand(TValue TComponent::* fieldPtr, const TValue& valueOld, const TValue& valueNew)
{
	m_Commands.Append(new ComponentField<TComponent, TValue>(fieldPtr, m_EntityUUID, valueOld, valueNew));
}