#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorInspectorSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/FileHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/TemplateManager.h"
#include "Engine/UUIDComponent.h"
#include "Engine/UUIDHelpers.h"
#include "EntityEditor/EntityEditorCommandManager.h"
#include "EntityEditor/EntityEditorInspector.h"
#include "EntityEditor/EntityEditorInspectorComponent.h"
#include "EntityEditor/EntityEditorOpenInspectorEvent.h"
#include "EntityEditor/EntityEditorSelectComponent.h"
#include "EntityEditor/EntityEditorSettingsComponent.h"
#include "EntityEditor/EntityEditorTemplates.h"
#include "GameState/GameStateEditorComponent.h"
#include "Serialize/Visitor.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_user.h"

namespace
{
	const str::Name strInput = str::Name::Create("Inspector");
	const str::Name strSave = str::Name::Create("Inspector_Save");
	const str::Name strSprite = str::Name::Create("Sprite");

	using World = editor::entity::InspectorSystem::World;
	using WindowView = ecs::EntityView
		::Required<
		editor::entity::InspectorComponent>;

	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}

	template<typename TComponent>
	void InspectComponent(ecs::EntityWorld& world, const ecs::Entity& entity, editor::entity::Inspector& inspector)
	{
		static const str::String name = str::String(TypeName<TComponent>::m_WithNamespace);
		if (world.HasComponent<TComponent>(entity))
			inspector.Visit(name.c_str(), world.ReadComponent<TComponent>(entity));
	}

	template<typename TComponent>
	void ToggleComponent(ecs::EntityWorld& world, const ecs::Entity& entity)
	{
		static const str::String name = str::String(TypeName<TComponent>::m_WithNamespace);
		const bool hasComponent = world.HasComponent<TComponent>(entity);
		imgui::Checkbox("##has", hasComponent);
		ImGui::SameLine();
		ImGui::BeginDisabled(entity.IsUnassigned());
		if (ImGui::MenuItem(name.c_str()))
		{
			const str::Guid entityUUID = eng::ToUUID(world, entity);
			if (!hasComponent)
			{
				auto& commands = world.WriteResource<editor::entity::CommandManager>();
				commands.ComponentAdd<TComponent>(entityUUID);
			}
			else
			{
				auto& commands = world.WriteResource<editor::entity::CommandManager>();
				commands.ComponentRemove<TComponent>(entityUUID);
			}
		}
		ImGui::EndDisabled();
	}

	void Draw_Components(ecs::EntityWorld& world, const ecs::Entity& entity)
	{
		ToggleComponent<camera::Move2DTemplate>(world, entity);
		ToggleComponent<camera::Move3DTemplate>(world, entity);
		ToggleComponent<camera::Pan3DTemplate>(world, entity);
		ToggleComponent<camera::Zoom2DTemplate>(world, entity);
		ToggleComponent<eng::CameraTemplate>(world, entity);
		ToggleComponent<eng::FlipbookTemplate>(world, entity);
		ToggleComponent<eng::PhysicsTemplate>(world, entity);
		ToggleComponent<eng::SpriteTemplate>(world, entity);
		ToggleComponent<eng::StaticMeshTemplate>(world, entity);
		ToggleComponent<eng::TransformTemplate>(world, entity);
	}

	void Draw_MenuBar(ecs::EntityWorld& world, const WindowView& view)
	{
		const auto& select = world.ReadComponent<editor::entity::SelectComponent>();
		const auto& window = view.ReadRequired<editor::entity::InspectorComponent>();

		const ecs::Entity selected = select.m_Entity;
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Components"))
			{
				Draw_Components(world, selected);
				ImGui::EndMenu();
			}

			auto& commands = world.WriteResource<editor::entity::CommandManager>();
			if (ImGui::MenuItem("Undo"))
				commands.UndoLastCommand();
			if (ImGui::MenuItem("Redo"))
				commands.RedoLastCommand();

			ImGui::EndMenuBar();
		}
	}

	void Draw_Inspector(ecs::EntityWorld& world, const WindowView& view)
	{
		const auto& select = world.ReadComponent<editor::entity::SelectComponent>();
		const auto& window = view.ReadRequired<editor::entity::InspectorComponent>();

		const ecs::Entity entity = select.m_Entity;
		const str::Guid entityUUID = eng::ToUUID(world, entity);
		if (!entityUUID.IsValid())
			return;

		editor::entity::Inspector inspector;
		inspector.Begin("", entityUUID);
		InspectComponent<ecs::NameComponent>(world, entity, inspector);
		InspectComponent<eng::UUIDComponent>(world, entity, inspector);
		InspectComponent<eng::VisibilityTemplate>(world, entity, inspector);
		InspectComponent<eng::TransformTemplate>(world, entity, inspector);
		//////////////////////////////////////////////////////////////////////////
		InspectComponent<camera::Move2DTemplate>(world, entity, inspector);
		InspectComponent<camera::Move3DTemplate>(world, entity, inspector);
		InspectComponent<camera::Pan3DTemplate>(world, entity, inspector);
		InspectComponent<camera::Zoom2DTemplate>(world, entity, inspector);
		InspectComponent<eng::CameraTemplate>(world, entity, inspector);
		InspectComponent<eng::FlipbookTemplate>(world, entity, inspector);
		InspectComponent<eng::PhysicsTemplate>(world, entity, inspector);
		InspectComponent<eng::SpriteTemplate>(world, entity, inspector);
		InspectComponent<eng::StaticMeshTemplate>(world, entity, inspector);
		inspector.End();

		if (inspector.HasCommands())
		{
			auto& manager = world.WriteResource<editor::entity::CommandManager>();
			manager.AddCommands(inspector.MoveCommands());
		}
	}

	void Draw_History(ecs::EntityWorld& world, const WindowView& view)
	{
		static str::String s_Scratch = {};
		const auto& commands = world.ReadResource<editor::entity::CommandManager>();
		const auto& undoStack = commands.GetUndoCommands();
		const auto& redoStack = commands.GetRedoCommands();

		ImGui::Selectable("##start", undoStack.IsEmpty());

		const int32 last = undoStack.GetCount() - 1;
		for (auto&& [i, command] : enumerate::Forward(undoStack))
		{
			const char* name = command->ToString();
			s_Scratch = std::format("{}##undo{}", name, i);
			ImGui::Selectable(s_Scratch.c_str(), i == last);
		}

		for (auto&& [i, command] : enumerate::Reverse(redoStack))
		{
			const char* name = command->ToString();
			s_Scratch = std::format("{}##redo{}", name, i);
			ImGui::Selectable(s_Scratch.c_str());
		}
	}

	void Draw_SavePopup(ecs::EntityWorld& world, const WindowView& view)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		const auto& input = world.ReadResource<eng::InputManager>();
		if (input.IsPressed(strSave))
		{
			const auto& select = world.ReadComponent<editor::entity::SelectComponent>();
			const auto& settings = world.ReadComponent<editor::entity::SettingsComponent>();

			const ecs::Entity selected = select.m_Entity;
			const auto& name = world.ReadComponent<ecs::NameComponent>(selected);

			str::Path filepath;
			if (filepath.IsEmpty())
			{
				eng::SaveFileSettings saveFile;
				saveFile.m_Title = "Save Entity";
				saveFile.m_Filters = { "Templates (*.template)", "*.template" };
				saveFile.m_Path = str::Path(settings.m_Save, name.m_Name, eng::TemplateManager::s_Extension);
				filepath = eng::SaveFileDialog(saveFile);
			}

			if (!filepath.IsEmpty())
			{
				auto& settings = world.WriteComponent<editor::entity::SettingsComponent>();
				settings.m_Save = filepath.GetDirectory();

				Visitor visitor;
				auto& manager = world.WriteResource<eng::TemplateManager>();
				manager.ReadEntity(world, selected, visitor);
				visitor.SaveToFile(filepath);
			}
		}
	}
}

editor::entity::InspectorSystem::InspectorSystem(ecs::EntityWorld& world)
	: m_World(world)
{
}

void editor::entity::InspectorSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!world.HasComponent<gamestate::EditorComponent>())
		return;

	if (world.HasAny<ecs::query::Added<editor::entity::InspectorComponent>>())
	{
		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::EditorUI;
		layer.m_Bindings.Emplace(strSave, input::EKey::S, input::EKey::Control_L);
		layer.m_Bindings.Emplace(strSave, input::EKey::S, input::EKey::Control_R);

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInput, layer);
	}

	using RemovedQuery = ecs::query
		::Condition<ecs::Alive, ecs::Dead>
		::Removed<editor::entity::InspectorComponent>;
	if (world.HasAny<RemovedQuery>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	for (const auto& request : world.Events<editor::entity::OpenInspectorEvent>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Inspector");

		auto& window = world.AddComponent<editor::entity::InspectorComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_Label = ToLabel("Inspector", identifier);
	}

	using RemovedQuery = ecs::query
		::Condition<ecs::Alive, ecs::Dead>
		::Removed<editor::entity::InspectorComponent>;
	for (auto&& view : world.Query<RemovedQuery>())
	{
		const auto& window = world.ReadComponent<editor::entity::InspectorComponent>(view, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (auto&& windowView : world.Query<ecs::query::Include<editor::entity::InspectorComponent>>())
	{
		constexpr ImGuiWindowFlags s_WindowFlags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_MenuBar;

		constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
		constexpr Vector2f s_DefaultSize = Vector2f(800, 600.f);

		const auto& window = windowView.WriteRequired<editor::entity::InspectorComponent>();

		bool isOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(window.m_Label.c_str(), &isOpen, s_WindowFlags))
		{
			Draw_MenuBar(m_World, windowView);
			if (ImGui::BeginTabBar("##options"))
			{
				if (ImGui::BeginTabItem("Inspector"))
				{
					Draw_Inspector(m_World, windowView);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("History"))
				{
					Draw_History(m_World, windowView);
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			Draw_SavePopup(m_World, windowView);
		}
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(windowView);
	}

	auto& commands = world.WriteResource<editor::entity::CommandManager>();
	commands.ExecuteCommands(gameTime);
}