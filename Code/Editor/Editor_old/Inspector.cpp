#include "EditorPCH.h"
#include "Editor/Inspector.h"

#include "Editor/ComponentList.h"
#include "Editor/HistorianComponents.h"
#include "Editor/InspectorWidgets.h"
#include "Editor/SceneEditor.h"

#include <Core/TypeList.h>

#include <iostream>
#include <entt/entt.hpp>
#include <imgui/imgui.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

namespace editor
{
	// #todo: multiple windows
	// #todo: multi-edit
	// #todo: reset to default 
	// #todo: reset to parent
	// #todo: undo/redo

	static float s_DividerOffset = 250.f;

	template<typename Component>
	void InspectComponent(entt::registry& registry, ecs::Entity entity)
	{
		if (Component* component = registry.try_get<Component>(entity))
		{
			constexpr refl::type_descriptor descriptor = refl::reflect<Component>();
			const char* name = get_display_name(descriptor);

			if (ImGui::CollapsingHeader(name, ImGuiTreeNodeFlags_DefaultOpen))
			{
				imgui::InspectorBegin();
				editor::InspectType(*component);
				imgui::InspectorEnd();
			}
		}
	}

	template <typename ...Types>
	void InspectComponents(entt::registry& registry, ecs::Entity entity, core::TypeList<Types...> typeList)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 20.f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 8.f, 8.f });

		(InspectComponent<Types>(registry, entity), ...);

		ImGui::PopStyleVar(2);
	}
}

editor::Inspector::Inspector(
	editor::Historian& historian,
	editor::SceneEditor& sceneEditor)
	: m_Historian(historian)
	, m_SceneEditor(sceneEditor)
{
}

void editor::Inspector::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& registry = m_World->m_Registry;

	m_HasChanged |= !registry.view<editor::RedoEventComponent>().empty();
	m_HasChanged |= !registry.view<editor::UndoEventComponent>().empty();

	Render();
}

void editor::Inspector::Render()
{
	auto& registry = m_World->m_Registry;
	if (!IsVisible())
		return;

	if (ImGui::Begin("Inspector", &m_IsVisible, ImGuiWindowFlags_MenuBar))
	{
		Render_MenuBar(registry);
		Render_Selection(registry);
	}
	ImGui::End();
}

void editor::Inspector::Render_MenuBar(entt::registry& registry)
{
	if (ImGui::BeginMenuBar())
	{
		ImGui::EndMenuBar();
	}
}

void editor::Inspector::Render_Selection(entt::registry& registry)
{
	if (ImGui::BeginChild("entity"))
	{
		if (registry.valid(m_Selection.m_Value))
		{
			ImGui::PushID(static_cast<int32>(m_Selection.m_Value));

			//InspectComponents(registry, m_Selection, s_ComponentList);

			ImGui::PopID();
		}
	}
	ImGui::EndChild();
}