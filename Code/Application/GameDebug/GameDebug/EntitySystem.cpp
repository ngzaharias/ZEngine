#include "GameDebugPCH.h"
#include "GameDebug/EntitySystem.h"

#include "Core/Algorithms.h"
#include "Core/Colour.h"
#include "Core/Map.h"
#include "Core/Name.h"
#include "Core/String.h"
#include "Core/TypeInfo.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameDebug/DebugEntityWindowRequest.h"
#include "GameDebug/EntityWindowComponent.h"
#include "Math/Vector.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_user.h"

namespace
{
	str::String s_ScratchString = { };

	constexpr ImGuiDockNodeFlags s_DockNodeFlags =
		ImGuiDockNodeFlags_NoCloseButton |
		ImGuiDockNodeFlags_NoWindowMenuButton;
	constexpr ImGuiTableFlags s_EntitiesFlags = 0;
	constexpr ImGuiWindowFlags s_ComponentsFlags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_MenuBar;

	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}

	void DebugEntity(ecs::EntityWorld& entityWorld, const ecs::Entity& entity, ecs::Entity& inout_Selected)
	{
		int32 index = entity.GetIndex();
		int32 salt = entity.GetVersion();
		s_ScratchString = std::to_string(index) + ", " + std::to_string(salt);
		if (entityWorld.HasComponent<ecs::NameComponent>(entity))
		{
			const auto& nameComponent = entityWorld.ReadComponent<ecs::NameComponent>(entity);
			if (!nameComponent.m_Name.empty())
				s_ScratchString += " - " + nameComponent.m_Name;
		}

		const bool isSelected = entity == inout_Selected;
		if (imgui::BulletHeader(s_ScratchString.c_str(), isSelected))
			inout_Selected = isSelected ? ecs::Entity::Unassigned : entity;
	}

	void DebugEntities(ecs::EntityWorld& entityWorld, ecs::Entity& inout_Selected)
	{
		const auto& entities = entityWorld.m_EntityStorage.m_AliveEntities;
		for (const auto& [debugEntity, componentMask] : entities)
			DebugEntity(entityWorld, debugEntity, inout_Selected);
	}

	void DebugComponents(ecs::EntityWorld& world, const ecs::Entity& entity)
	{
		if (entity.IsUnassigned())
			return;
		if (!world.IsAlive(entity))
			return;

		const char* name = nullptr;
		if (world.HasComponent<ecs::NameComponent>(entity))
		{
			const auto& nameComponent = world.ReadComponent<ecs::NameComponent>(entity);
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
}

debug::EntitySystem::EntitySystem(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld)
	: m_ClientWorld(clientWorld)
	, m_ServerWorld(serverWorld)
{
}

void debug::EntitySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoCollapse;
	constexpr Vector2f s_DefaultPos = Vector2f(100.f, 100.f);
	constexpr Vector2f s_DefaultSize = Vector2f(300.f, 200.f);

	for (const auto& request : world.Events<debug::EntityWindowRequest>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Entity Debugger");

		auto& window = world.AddComponent<debug::EntityWindowComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_DockspaceLabel = ToLabel("Entity Debugger##entitydebugger", identifier);
		window.m_EntitiesLabel = ToLabel("Entities##entitydebugger", identifier);
		window.m_ComponentsLabel = ToLabel("Components##entitydebugger", identifier);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<const debug::EntityWindowComponent>>())
	{
		auto& window = world.ReadComponent<debug::EntityWindowComponent>(entity, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<debug::EntityWindowComponent>>())
	{
		auto& window = world.WriteComponent<debug::EntityWindowComponent>(windowEntity);

		bool isOpen = true;
		bool isClient = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(window.m_DockspaceLabel.c_str(), &isOpen, s_WindowFlags))
		{
			if (ImGui::BeginTabBar(""))
			{
				ImGui::PushStyleColor(ImGuiCol_TabSelected, Colour::Blue);
				ImGui::PushStyleColor(ImGuiCol_TabHovered, { 0.f, 0.f, 1.f, 1.f });
				if (ImGui::BeginTabItem("Client"))
				{
					isClient = true;
					ImGui::EndTabItem();
				}
				ImGui::PopStyleColor(2);

				ImGui::PushStyleColor(ImGuiCol_TabSelected, Colour::Red);
				ImGui::PushStyleColor(ImGuiCol_TabHovered, { 1.f, 0.f, 0.f, 1.f });
				if (ImGui::BeginTabItem("Server"))
				{
					isClient = false;
					ImGui::EndTabItem();
				}
				ImGui::PopStyleColor(2);

				ImGui::EndTabBar();
			}

			const ImGuiID dockspaceId = ImGui::GetID(window.m_DockspaceLabel.c_str());
			if (!ImGui::DockBuilderGetNode(dockspaceId))
			{
				ImGui::DockBuilderRemoveNode(dockspaceId);
				ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetWindowSize());

				ImGuiID entitiesId, componentsId;
				ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.6f, &entitiesId, &componentsId);
				ImGui::DockBuilderDockWindow(window.m_EntitiesLabel.c_str(), entitiesId);
				ImGui::DockBuilderDockWindow(window.m_ComponentsLabel.c_str(), componentsId);
				ImGui::DockBuilderFinish(dockspaceId);
			}
			ImGui::DockSpace(dockspaceId, ImVec2(0, 0), s_DockNodeFlags);
		}
		ImGui::End();

		if (ImGui::Begin(window.m_EntitiesLabel.c_str()))
		{
			isClient
				? DebugEntities(m_ClientWorld, window.m_ClientEntity)
				: DebugEntities(m_ServerWorld, window.m_ServerEntity);
		}
		ImGui::End();

		if (ImGui::Begin(window.m_ComponentsLabel.c_str()))
		{
			isClient
				? DebugComponents(m_ClientWorld, window.m_ClientEntity)
				: DebugComponents(m_ServerWorld, window.m_ServerEntity);
		}
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(windowEntity);
	}
}