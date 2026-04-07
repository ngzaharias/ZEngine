#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorInspectorSystem.h"

#include "Camera/CameraMove3DComponent.h"
#include "Command/ComponentUpdate.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/FileHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/InspectorHelpers.h"
#include "Engine/LevelEntityComponent.h"
#include "Engine/PhysicsComponent.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TemplateHelpers.h"
#include "Engine/TemplateManager.h"
#include "Engine/TransformComponent.h"
#include "Engine/VisibilityComponent.h"
#include "EntityEditor/EntityEditorCommands.h"
#include "EntityEditor/EntityEditorInspectorComponent.h"
#include "EntityEditor/EntityEditorOpenInspectorEvent.h"
#include "EntityEditor/EntityEditorSelectComponent.h"
#include "EntityEditor/EntityEditorSettingsComponent.h"
#include "GameState/GameStateEditorComponent.h"
#include "Serialize/Visitor.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_user.h"
#include "imgui/Inspector.h"

#include "Engine/TransformTemplate.h"

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

	template<typename T>
	const char* ToString(T)
	{
		static str::String typeName = str::String(TypeName<T>::m_NoNamespace);
		return typeName.c_str();
	}

	template<typename TComponent>
	void Draw_Component(ecs::EntityWorld& world, const ecs::Entity& entity)
	{
		const bool hasComponent = world.HasComponent<TComponent>(entity);
		imgui::Checkbox("##has", hasComponent);
		ImGui::SameLine();
		ImGui::BeginDisabled(entity.IsUnassigned());
		if (ImGui::MenuItem("eng::TemplateComponent"))
		{
			const str::Guid entityUUID = eng::ToGuid(world, entity);
			if (!hasComponent)
			{
				auto& commands = world.WriteResource<editor::entity::Commands>();
				commands.AddComponent<TComponent>(eng::ToGuid(world, entity));
			}
			else
			{
				auto& commands = world.WriteResource<editor::entity::Commands>();
				commands.RemoveComponent<TComponent>(eng::ToGuid(world, entity));
			}
		}
		ImGui::EndDisabled();
	}

	void Draw_Components(ecs::EntityWorld& world, const ecs::Entity& entity)
	{
		Draw_Component<eng::TransformTemplate>(world, entity);
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

			auto& commands = world.WriteResource<editor::entity::Commands>();
			if (ImGui::MenuItem("Undo"))
				commands.Undo();
			if (ImGui::MenuItem("Redo"))
				commands.Redo();

			ImGui::EndMenuBar();
		}
	}

	void Draw_Inspector(ecs::EntityWorld& world, const WindowView& view)
	{
		const auto& select = world.ReadComponent<editor::entity::SelectComponent>();
		const auto& window = view.ReadRequired<editor::entity::InspectorComponent>();

		const ecs::Entity entity = select.m_Entity;
		if (!world.IsAlive(entity))
			return;

		if (world.HasComponent<eng::TransformTemplate>(entity))
		{
			auto componentOld = world.ReadComponent<eng::TransformTemplate>(entity);
			auto componentNew = componentOld;
			if (imgui::DragVector("m_Translate", componentNew.m_Translate))
			{
				auto& commands = world.WriteResource<editor::entity::Commands>();
				commands.UpdateComponent(
					&eng::TransformTemplate::m_Translate,
					eng::ToGuid(world, entity),
					componentOld.m_Translate,
					componentNew.m_Translate);
			}

			if (imgui::DragRotator("m_Rotate", componentNew.m_Rotate))
			{
				auto& commands = world.WriteResource<editor::entity::Commands>();
				commands.UpdateComponent(
					&eng::TransformTemplate::m_Rotate,
					eng::ToGuid(world, entity),
					componentOld.m_Rotate,
					componentNew.m_Rotate);
			}

			if (imgui::DragVector("m_Scale", componentNew.m_Scale))
			{
				auto& commands = world.WriteResource<editor::entity::Commands>();
				commands.UpdateComponent(
					&eng::TransformTemplate::m_Scale,
					eng::ToGuid(world, entity),
					componentOld.m_Scale,
					componentNew.m_Scale);
			}
		}
	}

	void Draw_History(ecs::EntityWorld& world, const WindowView& view)
	{
		static str::String s_Scratch = {};

		auto& commands = world.WriteResource<editor::entity::Commands>();
		auto& undoStack = commands.GetUndos();
		auto& redoStack = commands.GetRedos();

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
			if (world.HasComponent<eng::TemplateComponent>(selected))
			{
				const auto& prototype = world.ReadComponent<eng::TemplateComponent>(selected);
				const auto& name = world.ReadComponent<ecs::NameComponent>(selected);

				str::Path filepath = prototype.m_Path;
				if (filepath.IsEmpty())
				{
					eng::SaveFileSettings saveFile;
					saveFile.m_Title = "Save Entity";
					saveFile.m_Filters = { "Templates (*.template)", "*.template" };
					saveFile.m_Path = str::Path(settings.m_Save, name.m_Name, eng::TemplateManager::s_Extension);
					filepath = eng::SaveFileDialog(saveFile);
				}

				//if (!filepath.IsEmpty())
				//{
				//	auto& settings = world.WriteComponent<editor::entity::SettingsComponent>();
				//	settings.m_Save = filepath.GetDirectory();

				//	auto& manager = world.WriteResource<eng::TemplateManager>();
				//	manager.ReadEntity(world, selected, filepath);
				//}
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

	if (world.HasAny<ecs::query::Removed<editor::entity::InspectorComponent>>())
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

	for (auto&& view : world.Query<ecs::query::Removed<const editor::entity::InspectorComponent>>())
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

	auto& commands = world.WriteResource<editor::entity::Commands>();
	commands.Update();
}