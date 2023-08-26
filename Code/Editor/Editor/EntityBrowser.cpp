#include "EditorPCH.h"
#include "Editor/EntityBrowser.h"

#include "Editor/Inspector.h"

#include <Core/SortHelpers.h>
#include <Core/StringHelpers.h>

#include <Engine/CameraComponent.h>
#include <Engine/FlipbookComponent.h>
#include <Engine/NameComponent.h>
#include <Engine/SpriteComponent.h>
#include <Engine/TransformComponent.h>

#include <entt/entity/registry.hpp>
#include <imgui/imgui.h>

namespace
{
	void InputText(const char* label, std::string& string)
	{
		const size_t size = 256;
		char* buffer = new char[size];
		strncpy_s(buffer, size, string.c_str(), size);

		ImGui::InputText(label, buffer, size);
		string = buffer;
	}
}

bool editor::EntityEntry::operator==(const EntityEntry& rhs) const
{
	return m_Name == rhs.m_Name;
}

bool editor::EntityEntry::operator<(const EntityEntry& rhs) const
{
	const int32 val = sort::AlphaNumeric(m_Name, rhs.m_Name);
	if (val == 0)
		return m_Entity < rhs.m_Entity;
	return val < 0;
}

editor::EntityBrowser::EntityBrowser(editor::Inspector& inspector)
	: m_Inspector(inspector)
{
}
editor::EntityBrowser::~EntityBrowser()
{
}

void editor::EntityBrowser::Initialise()
{
}

void editor::EntityBrowser::Destroy()
{
}

void editor::EntityBrowser::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& registry = m_World->m_Registry;

	m_Entries.clear();
	registry.each([&](const ecs::Entity& entity)
	{
		const uint32 value = static_cast<uint32>(entity.m_Value);
		str::String name;
		if (const auto* component = registry.try_get<core::NameComponent>(entity.m_Value))
		{
			name = component->m_Name;
		}
		else
		{
			name = "<unknown>";
		}

		m_Entries.insert({ entity, name });
	});

	Render();
}

void editor::EntityBrowser::Render()
{
	auto& registry = m_World->m_Registry;

	if (!m_IsVisible)
		return;

	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_MenuBar;
	flags |= ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin("Entity Browser", &m_IsVisible, flags))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Create"))
			{
				if (ImGui::MenuItem("Empty"))
					Command_CreateEmpty(registry);
				ImGui::Separator();
				if (ImGui::MenuItem("Camera"))
					Command_CreateCamera(registry);
				if (ImGui::MenuItem("Flipbook"))
					Command_CreateFlipbook(registry);
				if (ImGui::MenuItem("Sprite"))
					Command_CreateSprite(registry);

				ImGui::EndMenu();
			}

			ImGui::Separator();
			ImGui::Text("?");
			ImGui::SetNextItemWidth(-1.f);
			InputText("", m_Filter);

			ImGui::EndMenuBar();
		}

		for (const auto& entry : m_Entries)
		{
			if (!m_Filter.empty() && !str::Contains_NoCase(entry.m_Name, m_Filter))
				continue;

			ImGui::PushID(static_cast<int32>(entry.m_Entity.m_Value));

			if (ImGui::Selectable(entry.m_Name.c_str()))
				m_Inspector.SetSelection(entry.m_Entity);

			if (ImGui::BeginPopupContextItem(""))
			{
				if (ImGui::MenuItem("Delete"))
					registry.destroy(entry.m_Entity.m_Value);

				ImGui::EndPopup();
			}
			ImGui::PopID();
		}
	}
	ImGui::End();
}

void editor::EntityBrowser::Command_CreateCamera(entt::registry& registry)
{
	ecs::Entity entity = registry.create();
	registry.emplace<camera::CameraComponent>(entity.m_Value);
	registry.emplace<core::NameComponent>(entity.m_Value);
	registry.emplace<core::TransformComponent>(entity.m_Value);
}

void editor::EntityBrowser::Command_CreateEmpty(entt::registry& registry)
{
	ecs::Entity entity = registry.create();
	registry.emplace<core::NameComponent>(entity.m_Value);
	registry.emplace<core::TransformComponent>(entity.m_Value);
}

void editor::EntityBrowser::Command_CreateFlipbook(entt::registry& registry)
{
	ecs::Entity entity = registry.create();
	registry.emplace<core::NameComponent>(entity.m_Value);
	registry.emplace<core::TransformComponent>(entity.m_Value);
	registry.emplace<render::FlipbookComponent>(entity.m_Value);
}

void editor::EntityBrowser::Command_CreateSprite(entt::registry& registry)
{
	ecs::Entity entity = registry.create();
	registry.emplace<core::NameComponent>(entity.m_Value);
	registry.emplace<core::TransformComponent>(entity.m_Value);
	registry.emplace<render::SpriteComponent>(entity.m_Value);
}
