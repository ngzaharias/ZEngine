#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorInspectorSystem.h"

#include "Camera/CameraMove3DComponent.h"
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
#include "EntityEditor/EntityEditorEntityUpdatedEvent.h"
#include "EntityEditor/EntityEditorHistoryComponent.h"
#include "EntityEditor/EntityEditorHistoryRedoEvent.h"
#include "EntityEditor/EntityEditorHistoryUndoEvent.h"
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

namespace
{
	const str::Name strInput = str::Name::Create("Inspector");
	const str::Name strSave = str::Name::Create("Inspector_Save");
	const str::Name strSprite = str::Name::Create("Sprite");

	using World = editor::entity::InspectorSystem::World;
	using WindowView = ecs::EntityView
		::Required<
		editor::entity::InspectorComponent>;

	template<typename T>
	const char* ToString(T)
	{
		static str::String typeName = str::String(TypeName<T>::m_NoNamespace);
		return typeName.c_str();
	}

	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}

	void SelectComponent(ecs::EntityWorld& world, const ecs::Entity& entity)
	{
		ecs::EntityStorage& storage = world.m_EntityStorage;
		const auto& registry = world.ReadResource<ecs::TypeRegistry>();
		for (auto&& [localId, componentInfo] : registry.GetComponentMap())
		{
			if (!componentInfo.m_IsTemplate)
				continue;

			const ecs::TypeInfo& typeInfo = registry.GetTypeInfo(componentInfo.m_GlobalId);
			const bool hasComponent = registry.HasComponent(storage, localId, entity);
			imgui::Checkbox("##has", hasComponent);
			ImGui::SameLine();
			if (ImGui::MenuItem(typeInfo.m_Name.c_str()))
			{
				if (!hasComponent)
				{
					registry.AddComponent(storage, localId, entity);
				}
				else
				{
					registry.RemoveComponent(storage, localId, entity);
				}

				str::String data;
				const auto& manager = world.ReadResource<eng::TemplateManager>();
				manager.ReadEntity(world, entity, data);
				world.AddEvent<editor::entity::EntityUpdatedEvent>(entity, std::move(data));
			}
		}
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
				if (world.IsAlive(selected))
					SelectComponent(world, selected);
				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Undo"))
				world.AddEvent<editor::entity::HistoryUndoEvent>();
			if (ImGui::MenuItem("Redo"))
				world.AddEvent<editor::entity::HistoryRedoEvent>();

			ImGui::EndMenuBar();
		}
	}

	void Draw_Inspector(ecs::EntityWorld& world, const WindowView& view)
	{
		const auto& select = world.ReadComponent<editor::entity::SelectComponent>();
		const auto& window = view.ReadRequired<editor::entity::InspectorComponent>();

		const ecs::Entity selected = select.m_Entity;
		if (world.IsAlive(selected))
		{
			imgui::Inspector inspector;
			inspector.AddPayload(world.ReadResource<eng::AssetManager>());
			if (inspector.Begin("##table"))
			{
				str::String data;
				const auto& manager = world.ReadResource<eng::TemplateManager>();
				manager.ReadEntity(world, selected, data);
				if (manager.InspectEntity(world, selected, inspector))
				{
					world.AddEvent<editor::entity::EntityUpdatedEvent>(selected, std::move(data));
				}
				inspector.End();
			}
		}
	}

	void Draw_History(ecs::EntityWorld& world, const WindowView& view)
	{
		auto ToString = [](auto type) -> const char*
		{
			return ::ToString(type);
		};

		const auto& historyComponent = world.ReadComponent<editor::entity::HistoryComponent>();
		for (const editor::entity::HistoryData& history : historyComponent.m_RedoStack)
		{
			const char* command = std::visit(ToString, history);
			ImGui::Text("%s", command);
		}
		ImGui::Text("---------- REDO ----------");
		ImGui::Separator();
		ImGui::Text("---------- UNDO ----------");
		for (auto [i, history] : enumerate::Reverse(historyComponent.m_UndoStack))
		{
			const char* command = std::visit(ToString, history);
			ImGui::Text("%s", command);
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
			if (ImGui::BeginTabBar(""))
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
}