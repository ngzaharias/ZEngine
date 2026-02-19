#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorWindowSystem.h"

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
#include "Engine/PrototypeManager.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/VisibilityComponent.h"
#include "Engine/Visitor.h"
#include "EntityEditor/EntityEditorOpenWindowEvent.h"
#include "EntityEditor/EntityEditorSaveComponent.h"
#include "EntityEditor/EntityEditorSelectSingleton.h"
#include "EntityEditor/EntityEditorSettingsSingleton.h"
#include "EntityEditor/EntityEditorWindowComponent.h"
#include "GameState/GameStateEditorComponent.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_user.h"

template<>
bool imgui::Inspector::WriteCustom(ecs::NameComponent& value)
{
	return Write("m_Name", value.m_Name);
}

namespace
{
	const str::Name strInput = str::Name::Create("EntityEditor");
	const str::Name strSave = str::Name::Create("EntityEditor_Save");
	const str::Name strSprite = str::Name::Create("Sprite");

	using World = editor::entity::WindowSystem::World;
	using WindowView = ecs::EntityView
		::Required<
		editor::entity::WindowComponent>
		::Optional<
		editor::entity::SaveComponent>;

	ecs::Entity CreateEntity(ecs::EntityWorld& world, const str::StringView name, const str::Name& level)
	{
		const ecs::Entity entity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(entity, name);
		world.AddComponent<eng::level::EntityComponent>(entity, level);
		world.AddComponent<eng::TransformComponent>(entity);

		auto& prototypeComponent = world.AddComponent<eng::PrototypeComponent>(entity);
		prototypeComponent.m_Guid = str::Guid::Generate();

		auto& select = world.WriteSingleton<editor::entity::SelectSingleton>();
		select.m_Entity = entity;

		return entity;
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
			if (!componentInfo.m_IsPrototype)
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
			}
		}
	}

	void Draw_EntityMenu(ecs::EntityWorld& world, const str::Name& levelName)
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Create"))
			{
				if (ImGui::MenuItem("Entity"))
					CreateEntity(world, "Entity_", levelName);

				if (ImGui::MenuItem("Camera"))
				{
					const ecs::Entity entity = CreateEntity(world, "Camera", levelName);
					world.AddComponent<camera::Move3DComponent>(entity);
					world.AddComponent<eng::CameraComponent>(entity);
				}

				if (ImGui::MenuItem("Sprite"))
				{
					const ecs::Entity entity = CreateEntity(world, "SP_", levelName);
					world.AddComponent<eng::VisibilityComponent>(entity);
					auto& spriteComponent = world.AddComponent<eng::SpriteComponent>(entity);
					spriteComponent.m_Sprite = str::Guid::Create("52ffdca6bc1d64230eda0e2056e9662b");
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void Draw_EntityList(World& world)
	{
		const auto& select = world.ReadSingleton<editor::entity::SelectSingleton>();

		using Query = ecs::query
			::Include<const eng::PrototypeComponent>
			::Optional<const ecs::NameComponent>;
		for (auto&& view : world.Query<Query>())
		{
			imgui::RaiiID id(view.GetEntity().GetIndex());

			const char* name = "<unknown>";;
			if (const auto* component = view.ReadOptional<ecs::NameComponent>())
				name = component->m_Name.c_str();

			const bool isSelected = view == select.m_Entity;
			if (ImGui::Selectable(name, isSelected))
			{
				auto& select = world.WriteSingleton<editor::entity::SelectSingleton>();
				select.m_Entity = view;
			}

			ImGui::OpenPopupOnItemClick(name);
			if (ImGui::BeginPopup(name))
			{
				if (ImGui::Selectable("Destroy"))
					world.DestroyEntity(view);
				ImGui::EndPopup();
			}
		}
	}

	void Draw_EntityDrop(ecs::EntityWorld& world, const str::Name& levelName)
	{
		const auto* window = ImGui::GetCurrentWindow();

		ImRect bb;
		bb.Min = window->Pos;
		bb.Max = bb.Min + window->Size;
		if (ImGui::BeginDragDropTargetCustom(bb, window->ID))
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("eng::AssetFile"))
			{
				const eng::AssetFile& file = *(const eng::AssetFile*)payload->Data;
				if (file.m_Type == strSprite)
				{
					const ecs::Entity entity = CreateEntity(world, file.m_Name, levelName);
					world.AddComponent<eng::VisibilityComponent>(entity);
					auto& spriteComponent = world.AddComponent<eng::SpriteComponent>(entity);
					spriteComponent.m_Sprite = file.m_Guid;
				}
			}
			ImGui::EndDragDropTarget();
		}
	}

	void Draw_Inspector(ecs::EntityWorld& world, const WindowView& view)
	{
		const auto& select = world.ReadSingleton<editor::entity::SelectSingleton>();
		const auto& window = view.ReadRequired<editor::entity::WindowComponent>();

		if (ImGui::Begin(window.m_InspectorLabel.c_str(), nullptr, ImGuiWindowFlags_MenuBar))
		{
			const ecs::Entity selected = select.m_Entity;
			if (world.IsAlive(selected))
			{
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu("Components"))
					{
						SelectComponent(world, selected);
						ImGui::EndMenu();
					}
					ImGui::EndMenuBar();
				}

				imgui::Inspector inspector;
				inspector.AddPayload(world.ReadResource<eng::AssetManager>());
				if (inspector.Begin("##table"))
				{
					auto& manager = world.WriteResource<eng::PrototypeManager>();
					manager.InspectEntity(world, selected, inspector);
					inspector.End();
				}
			}
		}
		ImGui::End();
	}

	void Draw_SavePopup(ecs::EntityWorld& world, const WindowView& view)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (view.HasOptional<editor::entity::SaveComponent>())
			world.RemoveComponent<editor::entity::SaveComponent>(view);

		const auto& input = world.ReadResource<eng::InputManager>();
		if (input.IsPressed(strSave) || view.HasOptional<editor::entity::SaveComponent>())
		{
			const auto& select = world.ReadSingleton<editor::entity::SelectSingleton>();
			const auto& settings = world.ReadSingleton<editor::entity::SettingsSingleton>();

			const ecs::Entity selected = select.m_Entity;
			if (world.HasComponent<eng::PrototypeComponent>(selected))
			{
				const auto& prototype = world.ReadComponent<eng::PrototypeComponent>(selected);
				const auto& name = world.ReadComponent<ecs::NameComponent>(selected);

				str::Path filepath = prototype.m_Path;
				if (filepath.IsEmpty())
				{
					eng::SaveFileSettings saveFile;
					saveFile.m_Title = "Save Entity";
					saveFile.m_Filters = { "Prototypes (*.prototype)", "*.prototype" };
					saveFile.m_Path = str::Path(settings.m_Save, name.m_Name, eng::PrototypeManager::s_Extension);
					filepath = eng::SaveFileDialog(saveFile);
				}

				if (!filepath.IsEmpty())
				{
					auto& settings = world.WriteSingleton<editor::entity::SettingsSingleton>();
					settings.m_Save = filepath.GetDirectory();

					auto& manager = world.WriteResource<eng::PrototypeManager>();
					manager.SaveEntity(world, selected, filepath);
				}
			}
		}
	}
}

editor::entity::WindowSystem::WindowSystem(ecs::EntityWorld& world)
	: m_World(world)
{
}

void editor::entity::WindowSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!world.HasAny<ecs::query::Include<gamestate::EditorComponent>>())
		return;

	if (world.HasAny<ecs::query::Added<editor::entity::WindowComponent>>())
	{
		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::EditorUI;
		layer.m_Bindings.Emplace(strSave, input::EKey::S, input::EKey::Control_L);
		layer.m_Bindings.Emplace(strSave, input::EKey::S, input::EKey::Control_R);

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInput, layer);
	}

	if (world.HasAny<ecs::query::Removed<editor::entity::WindowComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	constexpr ImGuiDockNodeFlags s_DockNodeFlags =
		ImGuiDockNodeFlags_NoCloseButton |
		ImGuiDockNodeFlags_NoWindowMenuButton;
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_MenuBar;

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(800, 600.f);

	for (const auto& request : world.Events<editor::entity::OpenWindowEvent>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Entity Editor");

		auto& window = world.AddComponent<editor::entity::WindowComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_DockspaceLabel = ToLabel("Entity Editor##entityeditor", identifier);
		window.m_EntitiesLabel = ToLabel("Entities##entityeditor", identifier);
		window.m_InspectorLabel = ToLabel("Inspector##entityeditor", identifier);
	}

	for (auto&& view : world.Query<ecs::query::Removed<const editor::entity::WindowComponent>>())
	{
		const auto& window = world.ReadComponent<editor::entity::WindowComponent>(view, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (auto&& windowView : world.Query<ecs::query::Include<editor::entity::WindowComponent>::Optional<editor::entity::SaveComponent>>())
	{
		const auto& window = windowView.WriteRequired<editor::entity::WindowComponent>();

		bool isOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(window.m_DockspaceLabel.c_str(), &isOpen, s_WindowFlags))
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Save", "Ctrl + S"))
						world.AddComponent<editor::entity::SaveComponent>(windowView);

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			const ImGuiID dockspaceId = ImGui::GetID(window.m_DockspaceLabel.c_str());
			if (!ImGui::DockBuilderGetNode(dockspaceId))
			{
				ImGui::DockBuilderRemoveNode(dockspaceId);
				ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetWindowSize());

				ImGuiID entitiesId, inspectorId;
				ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.3f, &entitiesId, &inspectorId);
				ImGui::DockBuilderDockWindow(window.m_EntitiesLabel.c_str(), entitiesId);
				ImGui::DockBuilderDockWindow(window.m_InspectorLabel.c_str(), inspectorId);
				ImGui::DockBuilderFinish(dockspaceId);
			}
			ImGui::DockSpace(dockspaceId, ImVec2(0, 0), s_DockNodeFlags);
		}
		ImGui::End();

		if (ImGui::Begin(window.m_EntitiesLabel.c_str(), nullptr, s_WindowFlags))
		{
			str::Name levelName = {};
			for (auto&& view : world.Query<ecs::query::Include<const eng::level::EntityComponent>>())
			{
				const auto& level = view.ReadRequired<eng::level::EntityComponent>();
				levelName = level.m_Name;
			}

			Draw_EntityMenu(m_World, levelName);
			Draw_EntityList(world);
			Draw_EntityDrop(m_World, levelName);
		}
		ImGui::End();

		Draw_Inspector(m_World, windowView);
		Draw_SavePopup(m_World, windowView);

		if (!isOpen)
			world.DestroyEntity(windowView);
	}
}