#include "EditorPCH.h"
#include "Editor/SpellEditor.h"

#include <Core/VariantHelpers.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <GameClient/SpellComponents.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_user.h>

namespace
{
	constexpr ImGuiDockNodeFlags s_DockNodeFlags =
		ImGuiDockNodeFlags_NoCloseButton |
		ImGuiDockNodeFlags_NoWindowMenuButton;
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_MenuBar;

	str::String ToLabel(const char* label, const ecs::Entity& entity)
	{
		return std::format("{}: {}", label, entity.GetIndex());
	}

	using World = editor::SpellEditor::World;
	bool DrawChildren(spell::RootDefinition& rootSpell, Array<spell::SubIndex>& children)
	{
		constexpr ImVec2 s_ItemSize = ImVec2(50.f, 50.f);

		bool toCreate = false;
		for (const spell::SubIndex subIndex : children)
		{
			ImGui::PushID(subIndex);
			spell::SubDefinition& subSpell = rootSpell.m_SubDefinitions[subIndex];

			str::String subLabel;
			core::VariantMatch(subSpell.m_Evocation,
				[&subLabel](const spell::Damage& data) { subLabel = "Damage"; },
				[&subLabel](const spell::Explosion& data) { subLabel = "Explosion"; },
				[&subLabel](const spell::Projectile& data) { subLabel = "Projectile"; });

			if (ImGui::CollapsingHeader(subLabel.c_str()))
			{
				ImGui::Indent();
				toCreate |= DrawChildren(rootSpell, subSpell.m_Children);
				ImGui::Unindent();
			}

			ImGui::PopID();
		}

		if (imgui::BulletHeader(" + "))
		{
			const spell::SubIndex subIndex = rootSpell.m_SubDefinitions.GetCount();
			children.Append(subIndex);
			return true;
		}

		return toCreate;
	}

	void DrawInspector(World& world)
	{
		auto& definitionComponent = world.GetSingleton<spell::DefinitionMapComponent>();
		auto& spells = definitionComponent.m_GuidToSpell;

		if (ImGui::Button("Create"))
			spells[str::Guid::Generate()];
		ImGui::SameLine();
		if (ImGui::Button("RemoveAll"))
			spells.RemoveAll();

		for (auto&& [guid, rootSpell] : spells)
		{
			const str::String rootLabel = guid.ToString();
			ImGui::PushID(rootLabel.c_str());

			if (ImGui::CollapsingHeader(rootLabel.c_str()))
			{
				ImGui::Indent();
				if (DrawChildren(rootSpell, rootSpell.m_Children))
					rootSpell.m_SubDefinitions.Emplace();
				ImGui::Unindent();
			}

			ImGui::PopID();
		}
	}
}

void editor::SpellEditor::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(1080, 800.f);

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const editor::SpellWindowRequestComponent>>())
	{
		const ecs::Entity windowEntity = world.CreateEntity();
		auto& windowComponent = world.AddComponent<editor::SpellWindowComponent>(windowEntity);
		windowComponent.m_DockspaceLabel = ToLabel("Spell Editor", windowEntity);
		windowComponent.m_InspectorLabel = ToLabel("Inspector", windowEntity);
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<editor::SpellWindowComponent>>())
	{
		auto& windowComponent = world.GetComponent<editor::SpellWindowComponent>(windowEntity);

		bool isOpen = true;
		ImGui::SetNextWindowPos({ s_DefaultPos.x, s_DefaultPos.y }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ s_DefaultSize.x, s_DefaultSize.y }, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(windowComponent.m_DockspaceLabel.c_str(), &isOpen, s_WindowFlags))
		{
			const ImGuiID dockspaceId = ImGui::GetID(windowComponent.m_DockspaceLabel.c_str());
			if (!ImGui::DockBuilderGetNode(dockspaceId))
			{
				ImGui::DockBuilderRemoveNode(dockspaceId);
				ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetWindowSize());

				//ImGuiID inspectorId, plottingId;
				//ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.7f, &plottingId, &inspectorId);
				ImGui::DockBuilderDockWindow(windowComponent.m_InspectorLabel.c_str(), dockspaceId);
				//ImGui::DockBuilderDockWindow(windowComponent.m_PlottingLabel.c_str(), plottingId);
				ImGui::DockBuilderFinish(dockspaceId);
			}
			ImGui::DockSpace(dockspaceId, ImVec2(0, 0), s_DockNodeFlags);
		}
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_InspectorLabel.c_str()))
			DrawInspector(world);
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(windowEntity);
	}
}