#include "EditorPCH.h"
#include "Editor/Historian.h"

#include "Editor/ComponentList.h"
#include "Editor/HistorianComponents.h"
#include "Editor/Transaction.h"

#include <Engine/InputComponent.h>

#include <imgui/imgui.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include <iostream>

void editor::Historian::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& registry = m_World->m_Registry;

	for (const ecs::Entity& entity : registry.view<editor::RedoEventComponent>())
		m_World->DestroyEntity(entity);
	for (const ecs::Entity& entity : registry.view<editor::UndoEventComponent>())
		m_World->DestroyEntity(entity);

	for (const ecs::Entity& entity : registry.view<input::InputComponent>())
	{
		const auto& component = m_World->GetComponent<input::InputComponent>(entity);
		if (component.IsKeyHeld(input::EKeyboard::Control_L) && component.IsKeyPressed(input::EKeyboard::Z))
		{
			if (component.IsKeyHeld(input::EKeyboard::Shift_L))
			{
				RedoRecord();
			}
			else
			{
				UndoRecord();
			}
		}

		if (component.IsKeyHeld(input::EKeyboard::Control_L) && component.IsKeyPressed(input::EKeyboard::Y))
			RedoRecord();
	}

	Render();
}

void editor::Historian::PushChange(const Record& record)
{
	m_UndoRecords.Push(record);
	m_RedoRecords.RemoveAll();
}

void editor::Historian::RedoRecord()
{
	if (!m_RedoRecords.IsEmpty())
	{
		Record recordApply = std::move(m_RedoRecords.Pop());
		Record recordStore = { }; // #todo: get current value
		m_UndoRecords.Push(recordStore);

		// #todo: apply changes

		ecs::Entity entity = m_World->CreateEntity();
		m_World->AddComponent<editor::RedoEventComponent>(entity);
	}
}

void editor::Historian::UndoRecord()
{
	if (!m_UndoRecords.IsEmpty())
	{
		Record recordApply = std::move(m_UndoRecords.Pop());
		Record recordStore = { }; // #todo: get current value
		m_RedoRecords.Push(recordStore);

		// #todo: apply changes

		ecs::Entity entity = m_World->CreateEntity();
		m_World->AddComponent<editor::UndoEventComponent>(entity);
	}
}

void editor::Historian::Render()
{
	auto& registry = m_World->m_Registry;

	if (!IsVisible())
		return;

	if (ImGui::Begin("Historian", &m_IsVisible, ImGuiWindowFlags_MenuBar))
	{
		ImGui::BeginTable("Historian", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable);
		ImGui::TableNextRow();

		// undo
		{
			ImGui::TableSetColumnIndex(0);
			if (ImGui::Button("Undo") && !m_UndoRecords.IsEmpty())
				UndoRecord();
			ImGui::Separator();

			for (int32 i = 0; i < m_UndoRecords.GetCount(); ++i)
				ImGui::Text("Resource: [%s]", m_UndoRecords[i].m_Resource.ToString().c_str());
		}

		// redo
		{
			ImGui::TableSetColumnIndex(1);
			if (ImGui::Button("Redo") && !m_RedoRecords.IsEmpty())
				RedoRecord();
			ImGui::Separator();

			for (int32 i = 0; i < m_RedoRecords.GetCount(); ++i)
				ImGui::Text("Resource: [%s]", m_RedoRecords[i].m_Resource.ToString().c_str());
		}

		ImGui::EndTable();
	}
	ImGui::End();
}
