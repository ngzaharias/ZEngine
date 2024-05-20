#include "EditorPCH.h"
#include "Editor/TransformEditor.h"

#include <Core/Rotator.h>
#include <Core/Vector.h>

#include <ECS/EntityWorld.h>
#include <ECS/NameComponent.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Engine/FileHelpers.h>
#include <Engine/InputComponent.h>
#include <Engine/PrototypeManager.h>
#include <Engine/TransformComponent.h>
#include <Engine/Visitor.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_user.h>

namespace
{
	struct Transform
	{
		Vector3f m_Translate = Vector3f::Zero;
		Rotator m_Rotate = Rotator::Zero;
		Vector3f m_Scale = Vector3f::One;
	};
}

template<>
void eng::Visitor::VisitCustom<Transform>(Transform& value)
{
	Visit("m_Translate", value.m_Translate, {});
	Visit("m_Rotate", value.m_Rotate, {});
	Visit("m_Scale", value.m_Scale, {});
}

namespace
{
	using World = editor::TransformEditor::World;

	constexpr ImGuiDockNodeFlags s_DockNodeFlags =
		ImGuiDockNodeFlags_NoCloseButton |
		ImGuiDockNodeFlags_NoWindowMenuButton;
	constexpr ImGuiTableFlags s_InspectorFlags = 0;
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_MenuBar;

	str::String ToLabel(const char* label, const ecs::Entity& entity)
	{
		return std::format("{}: {}", label, entity.GetIndex());
	}

	const char* ToName(World& world, const ecs::Entity& entity)
	{
		if (world.HasComponent<ecs::NameComponent>(entity))
		{
			const auto& name = world.GetComponent<const ecs::NameComponent>(entity);
			return name.m_Name.c_str();
		}
		return "<unknown>";
	}

	bool HasInput_Save(World& world)
	{
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::InputComponent>>())
		{
			const auto& input = world.GetComponent<const eng::InputComponent>(entity);
			return input.IsKeyHeld(input::EKeyboard::Control_L) && input.IsKeyPressed(input::EKeyboard::S);
		}
		return false;
	}

	void DrawMenuBar(World& world, const ecs::Entity& entity)
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save", "Ctrl + S"))
					world.AddComponent<editor::TransformAssetSaveComponent>(entity);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	};

	void DrawEntities(World& world, const ecs::Entity& entity)
	{
		auto& windowComponent = world.GetComponent<editor::TransformWindowComponent>(entity);

		using Query = ecs::query::Include<const eng::PrototypeComponent, const eng::TransformComponent>;
		for (auto&& view : world.Query<Query>())
		{
			const char* name = ToName(world, view);
			const bool isSelected = view == windowComponent.m_Selected;
			if (ImGui::Selectable(name, isSelected))
				windowComponent.m_Selected = view;
		}
	}

	void DrawInspector(World& world, const ecs::Entity& entity)
	{
		auto& windowComponent = world.GetComponent<editor::TransformWindowComponent>(entity);

		const ecs::Entity selected = windowComponent.m_Selected;
		if (world.HasComponent<eng::TransformComponent>(selected))
		{
			auto& transform = world.GetComponent<eng::TransformComponent>(selected);
			imgui::DragVector("m_Translate", transform.m_Translate);
			imgui::DragRotator("m_Rotate", transform.m_Rotate);
			imgui::DragVector("m_Scale", transform.m_Scale);
		}
	}

	void DrawPopupSave(World& world, const ecs::Entity& entity)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		const auto& prototypeManager = world.GetResource<const eng::PrototypeManager>();

		if (world.HasComponent<editor::TransformAssetSaveComponent>(entity))
			world.RemoveComponent<editor::TransformAssetSaveComponent>(entity);

		if (world.HasComponent<editor::TransformAssetSaveComponent>(entity) || HasInput_Save(world))
		{
			auto& windowComponent = world.GetComponent<editor::TransformWindowComponent>(entity);
			const ecs::Entity selected = windowComponent.m_Selected;

			const bool hasPrototype = world.HasComponent<eng::PrototypeComponent>(selected);
			const bool hasTransform = world.HasComponent<eng::TransformComponent>(selected);
			if (hasPrototype && hasTransform)
			{
				const auto& prototypeComponent = world.GetComponent<const eng::PrototypeComponent>(selected);
				const auto& transformComponent = world.GetComponent<const eng::TransformComponent>(selected);

				eng::SaveFileSettings settings;
				settings.m_Filters = { "Prototypes (*.prototype)", "*.prototype" };
				settings.m_Path = prototypeComponent.m_Path;

				const str::Path filepath = eng::SaveFileDialog(settings);
				if (!filepath.IsEmpty())
				{
					Transform transform;
					transform.m_Translate = transformComponent.m_Translate;
					transform.m_Rotate = transformComponent.m_Rotate;
					transform.m_Scale = transformComponent.m_Scale;

					eng::Visitor visitor;
					visitor.LoadFromFile(filepath);
					visitor.SetMode(eng::Visitor::Write);
					visitor.Visit("Transform", transform, {});

					str::String string = visitor;
					visitor.SaveToFile(filepath);
				}
			}
		}
	};
}

void editor::TransformEditor::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(1080, 800.f);

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const editor::TransformWindowRequestComponent>>())
	{
		const ecs::Entity windowEntity = world.CreateEntity();
		auto& windowComponent = world.AddComponent<editor::TransformWindowComponent>(windowEntity);
		windowComponent.m_DockspaceLabel = ToLabel("Texture Editor", windowEntity);
		windowComponent.m_EntitiesLabel  = ToLabel("Entities", windowEntity);
		windowComponent.m_InspectorLabel = ToLabel("Inspector", windowEntity);
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<editor::TransformWindowComponent>>())
	{
		auto& windowComponent = world.GetComponent<editor::TransformWindowComponent>(windowEntity);

		bool isOpen = true;
		ImGui::SetNextWindowPos({ s_DefaultPos.x, s_DefaultPos.y }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ s_DefaultSize.x, s_DefaultSize.y }, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(windowComponent.m_DockspaceLabel.c_str(), &isOpen, s_WindowFlags))
		{
			DrawMenuBar(world, windowEntity);

			const ImGuiID dockspaceId = ImGui::GetID(windowComponent.m_DockspaceLabel.c_str());
			if (!ImGui::DockBuilderGetNode(dockspaceId))
			{
				ImGui::DockBuilderRemoveNode(dockspaceId);
				ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetWindowSize());

				ImGuiID entitiesId, inspectorId;
				ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.6f, &entitiesId, &inspectorId);
				ImGui::DockBuilderDockWindow(windowComponent.m_EntitiesLabel.c_str(), entitiesId);
				ImGui::DockBuilderDockWindow(windowComponent.m_InspectorLabel.c_str(), inspectorId);
				ImGui::DockBuilderFinish(dockspaceId);
			}
			ImGui::DockSpace(dockspaceId, ImVec2(0, 0), s_DockNodeFlags);
		}
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_EntitiesLabel.c_str()))
			DrawEntities(world, windowEntity);
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_InspectorLabel.c_str()))
			DrawInspector(world, windowEntity);
		ImGui::End();

		DrawPopupSave(world, windowEntity);

		if (!isOpen)
			world.DestroyEntity(windowEntity);
	}
}