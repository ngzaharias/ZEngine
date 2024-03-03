#include "GameDebugPCH.h"
#include "GameDebug/EntitySystem.h"

#include <Core/Algorithms.h>
#include <Core/Map.h>
#include <Core/Name.h>
#include <Core/String.h>
#include <Core/Vector.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Engine/NameComponent.h>
#include <Engine/TypeInfo.h>

#include <imgui/imgui.h>
#include <imgui/imgui_user.h>

namespace
{
	str::String s_ScratchString = { };

	void DebugEntity(ecs::EntityWorld& entityWorld, const ecs::Entity& entity, ecs::Entity& inout_Selected)
	{
		int32 index = entity.GetIndex();
		int32 salt = entity.GetVersion();
		s_ScratchString = std::to_string(index) + ", " + std::to_string(salt);
		if (entityWorld.HasComponent<eng::NameComponent>(entity))
		{
			const auto& nameComponent = entityWorld.GetComponent<const eng::NameComponent>(entity);
			if (!nameComponent.m_Name.empty())
				s_ScratchString += " - " + nameComponent.m_Name;
		}

		const bool isSelected = entity == inout_Selected;
		if (imgui::BulletHeader(s_ScratchString.c_str(), isSelected))
			inout_Selected = isSelected ? ecs::Entity::Unassigned : entity;
	}

	void DebugComponents(ecs::EntityWorld& world, const ecs::Entity& entity)
	{
		const char* name = nullptr;
		if (world.HasComponent<eng::NameComponent>(entity))
		{
			const auto& nameComponent = world.GetComponent<const eng::NameComponent>(entity);
			name = nameComponent.m_Name.c_str();
		}

		ImGui::Text("%i, %i - %s", entity.GetIndex(), entity.GetVersion(), name);
		ImGui::Separator();

		const ecs::ComponentMask componentMask = world.m_EntityStorage.m_AliveEntities.Get(entity);
		for (int32 i = 0; i < ecs::COMPONENTS_MAX; ++i)
		{
			if (!componentMask.Has(i))
				continue;

			const ecs::ComponentEntry& entry = world.m_ComponentRegistry.GetEntry(i);
			imgui::BulletHeader(entry.m_Name.c_str());
		}
	}

	void DebugWorld(ecs::EntityWorld& entityWorld, ecs::Entity& inout_Selected)
	{
		const auto flags =
			ImGuiTableFlags_Resizable |
			ImGuiTableFlags_BordersInnerV |
			ImGuiTableFlags_ScrollY;
		if (ImGui::BeginTable("table", 2, flags))
		{
			ImGui::TableSetupColumn("Entity");
			ImGui::TableSetupColumn("Component");
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();

			// entity
			ImGui::TableSetColumnIndex(0);
			{
				ImGui::Text("Index, Version  Name");
				ImGui::Separator();

				const auto& entities = entityWorld.m_EntityStorage.m_AliveEntities;
				for (const auto& [debugEntity, componentMask] : entities)
					DebugEntity(entityWorld, debugEntity, inout_Selected);
			}

			// components
			ImGui::TableSetColumnIndex(1);
			{
				if (!inout_Selected.IsUnassigned() && entityWorld.IsAlive(inout_Selected))
					DebugComponents(entityWorld, inout_Selected);
			}

			ImGui::EndTable();
		}
	}
}

dbg::EntitySystem::EntitySystem(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld)
	: m_ClientWorld(clientWorld)
	, m_ServerWorld(serverWorld)
{
}

void dbg::EntitySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(100.f, 100.f);
	constexpr Vector2f s_DefaultSize = Vector2f(300.f, 200.f);

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const dbg::EntityWindowRequestComponent>>())
	{
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<eng::NameComponent>(windowEntity, "Debug: Entity Debugger");
		world.AddComponent<dbg::EntityWindowComponent>(windowEntity);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<dbg::EntityWindowComponent>>())
	{
		auto& component = world.GetComponent<dbg::EntityWindowComponent>(entity);
		const str::String label = "Debug: Entities##" + std::to_string(entity.GetIndex());

		bool isOpen = true;
		ImGui::SetNextWindowPos({ s_DefaultPos.x, s_DefaultPos.y }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ s_DefaultSize.x, s_DefaultSize.y }, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(label.c_str(), &isOpen))
		{
			if (ImGui::BeginTabBar(""))
			{
				ImGui::PushStyleColor(ImGuiCol_TabActive, { 0.f, 0.f, 1.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_TabHovered, { 0.f, 0.f, 1.f, 1.f });
				if (ImGui::BeginTabItem("Client"))
				{
					DebugWorld(m_ClientWorld, component.m_ClientEntity);
					ImGui::EndTabItem();
				}
				ImGui::PopStyleColor(2);

				ImGui::PushStyleColor(ImGuiCol_TabActive, { 1.f, 0.f, 0.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_TabHovered, { 1.f, 0.f, 0.f, 1.f });
				if (ImGui::BeginTabItem("Server"))
				{
					DebugWorld(m_ServerWorld, component.m_ServerEntity);
					ImGui::EndTabItem();
				}
				ImGui::PopStyleColor(2);

				ImGui::EndTabBar();
			}
		}
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(entity);
	}
}