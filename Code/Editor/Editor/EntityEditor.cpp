#include "EditorPCH.h"
#include "Editor/EntityEditor.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/SettingsComponents.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/FileHelpers.h"
#include "Engine/InputComponent.h"
#include "Engine/InspectorHelpers.h"
#include "Engine/LevelComponents.h"
#include "Engine/PhysicsComponent.h"
#include "Engine/PrototypeManager.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/Visitor.h"
#include "GameClient/HiddenObjectComponents.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_user.h>

template<>
bool imgui::Inspector::WriteCustom(ecs::NameComponent& value)
{
	return Write("m_Name", value.m_Name);
}

namespace
{
	using World = editor::EntityEditor::World;

	constexpr ImGuiDockNodeFlags s_DockNodeFlags =
		ImGuiDockNodeFlags_NoCloseButton |
		ImGuiDockNodeFlags_NoWindowMenuButton;
	constexpr ImGuiTableFlags s_InspectorFlags = 0;
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_MenuBar;

	ecs::Entity CreateEntity(ecs::EntityWorld& world, const str::StringView name, const str::Name& level)
	{
		const ecs::Entity entity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(entity, name);
		world.AddComponent<eng::level::EntityComponent>(entity, level);
		world.AddComponent<eng::TransformComponent>(entity);

		auto& prototypeComponent = world.AddComponent<eng::PrototypeComponent>(entity);
		prototypeComponent.m_Guid = str::Guid::Generate();

		return entity;
	}

	str::String ToLabel(const char* label, const ecs::Entity& entity)
	{
		return std::format("{}: {}", label, entity.GetIndex());
	}

	bool HasInput(World& world, const input::EKeyboard key)
	{
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::InputComponent>>())
		{
			const auto& input = world.ReadComponent<eng::InputComponent>(entity);
			return input.IsKeyHeld(input::EKeyboard::Control_L) && input.IsKeyPressed(key);
		}
		return false;
	}

	template<typename Component>
	void SelectComponent(ecs::EntityWorld& world, const ecs::Entity& entity)
	{
		static const str::String label = str::String(TypeName<Component>::m_WithNamespace);

		const bool hasComponent = world.HasComponent<Component>(entity);
		imgui::Checkbox("##has", hasComponent);
		ImGui::SameLine();
		if (ImGui::MenuItem(label.c_str()))
		{
			if (!hasComponent)
				world.AddComponent<Component>(entity);
			else
				world.RemoveComponent<Component>(entity);
		}
	}

	template<typename Component>
	void SaveComponent(ecs::EntityWorld& world, const ecs::Entity& entity, eng::Visitor& visitor)
	{
		if (world.HasComponent<Component>(entity))
		{
			static const str::StringView key = TypeName<Component>::m_WithNamespace;
			const auto& component = world.ReadComponent<Component>(entity);
			visitor.Write(key, component);
		}
	}

	template<typename Component>
	void InspectComponent(ecs::EntityWorld& world, const ecs::Entity& entity, imgui::Inspector& inspector)
	{
		if (world.HasComponent<Component>(entity))
		{
			static const str::String label = str::String(TypeName<Component>::m_WithNamespace);
			inspector.Write(label.c_str(), world.WriteComponent<Component>(entity));
		}
	}
}

editor::EntityEditor::EntityEditor(ecs::EntityWorld& world)
	: m_World(world)
{
}

void editor::EntityEditor::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(800, 600.f);

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const editor::EntityWindowRequestComponent>>())
	{
		const ecs::Entity windowEntity = world.CreateEntity();
		auto& windowComponent = world.AddComponent<editor::EntityWindowComponent>(windowEntity);
		windowComponent.m_DockspaceLabel = ToLabel("Entity Editor", windowEntity);
		windowComponent.m_EntitiesLabel  = ToLabel("Entities", windowEntity);
		windowComponent.m_InspectorLabel = ToLabel("Inspector", windowEntity);
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<editor::EntityWindowComponent>>())
	{
		auto& windowComponent = world.WriteComponent<editor::EntityWindowComponent>(windowEntity);

		bool isOpen = true;
		ImGui::SetNextWindowPos({ s_DefaultPos.x, s_DefaultPos.y }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ s_DefaultSize.x, s_DefaultSize.y }, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(windowComponent.m_DockspaceLabel.c_str(), &isOpen, s_WindowFlags))
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Save", "Ctrl + S"))
						world.AddComponent<editor::EntitySaveComponent>(windowEntity);

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

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

		if (ImGui::Begin(windowComponent.m_EntitiesLabel.c_str(), nullptr, s_WindowFlags))
		{
			str::Name levelName = {};
			for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::level::EntityComponent>>())
			{
				const auto& level = world.ReadComponent<eng::level::EntityComponent>(entity);
				levelName = level.m_Name;
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Create"))
				{
					if (ImGui::MenuItem("Entity"))
						CreateEntity(m_World, "Entity_", levelName);

					if (ImGui::MenuItem("Camera"))
					{
						const ecs::Entity entity = CreateEntity(m_World, "Camera", levelName);
						m_World.AddComponent<eng::camera::Move3DComponent>(entity);
						m_World.AddComponent<eng::camera::ProjectionComponent>(entity);
					}

					if (ImGui::MenuItem("Hidden Group"))
					{
						const ecs::Entity entity = CreateEntity(m_World, "Group_01", levelName);
						auto& groupComponent = m_World.AddComponent<hidden::GroupComponent>(entity);
						groupComponent.m_Objects.Emplace();
					}

					if (ImGui::MenuItem("Hidden Object"))
					{
						eng::ShapeBox defaultBox;
						defaultBox.m_Extents = Vector3f(20.f, 20.f, 1.f);

						const ecs::Entity entity = CreateEntity(m_World, "Object_01", levelName);
						auto& objectComponent = m_World.AddComponent<hidden::ObjectComponent>(entity);
						objectComponent.m_Sprite = str::Guid::Create("52ffdca6-bc1d-6423-0eda-0e2056e9662b");
						objectComponent.m_Size = Vector2u(128);

						auto& physicsComponent = m_World.AddComponent<eng::PhysicsComponent>(entity);
						physicsComponent.m_Rigidbody = eng::RigidStatic();
						physicsComponent.m_Shapes.Append(defaultBox);
					}

					if (ImGui::MenuItem("Sprite"))
					{
						const ecs::Entity entity = CreateEntity(m_World, "Sprite_01", levelName);
						auto& spriteComponent = m_World.AddComponent<eng::SpriteComponent>(entity);
						spriteComponent.m_Sprite = str::Guid::Create("52ffdca6-bc1d-6423-0eda-0e2056e9662b");
						spriteComponent.m_Size = Vector2u(128);
					}
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			using Query = ecs::query::Include<const eng::PrototypeComponent>;
			for (const ecs::Entity& entity : world.Query<Query>())
			{
				imgui::RaiiID id(entity.GetIndex());

				const char* name = "<unknown>";;
				if (m_World.HasComponent<ecs::NameComponent>(entity))
				{
					const auto& component = m_World.ReadComponent<ecs::NameComponent>(entity);
					name = component.m_Name.c_str();
				}

				const bool isSelected = entity == windowComponent.m_Selected;
				if (ImGui::Selectable(name, isSelected))
					windowComponent.m_Selected = entity;

				ImGui::OpenPopupOnItemClick(name);
				if (ImGui::BeginPopup(name))
				{
					if (ImGui::Selectable("Destroy"))
						world.DestroyEntity(entity);
					ImGui::EndPopup();
				}
			}
		}
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_InspectorLabel.c_str(), nullptr, ImGuiWindowFlags_MenuBar))
		{
			const ecs::Entity selected = windowComponent.m_Selected;
			if (world.IsAlive(selected))
			{
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu("Components"))
					{
						SelectComponent<ecs::NameComponent>(m_World, selected);
						SelectComponent<eng::camera::Bound2DComponent>(m_World, selected);
						SelectComponent<eng::camera::Move2DComponent>(m_World, selected);
						SelectComponent<eng::camera::Move3DComponent>(m_World, selected);
						SelectComponent<eng::camera::Pan3DComponent>(m_World, selected);
						SelectComponent<eng::camera::ProjectionComponent>(m_World, selected);
						SelectComponent<eng::camera::Zoom2DComponent>(m_World, selected);
						SelectComponent<eng::PhysicsComponent>(m_World, selected);
						SelectComponent<eng::PrototypeComponent>(m_World, selected);
						SelectComponent<eng::SpriteComponent>(m_World, selected);
						SelectComponent<eng::TransformComponent>(m_World, selected);
						SelectComponent<hidden::GroupComponent>(m_World, selected);
						SelectComponent<hidden::ObjectComponent>(m_World, selected);
						ImGui::EndMenu();
					}
					ImGui::EndMenuBar();
				}

				imgui::Inspector inspector;
				inspector.AddPayload(world.ReadResource<eng::AssetManager>());
				if (inspector.Begin("##table"))
				{
					// always first
					InspectComponent<ecs::NameComponent>(m_World, selected, inspector);
					InspectComponent<eng::TransformComponent>(m_World, selected, inspector);

					InspectComponent<eng::camera::Bound2DComponent>(m_World, selected, inspector);
					InspectComponent<eng::camera::Move2DComponent>(m_World, selected, inspector);
					InspectComponent<eng::camera::Move3DComponent>(m_World, selected, inspector);
					InspectComponent<eng::camera::Pan3DComponent>(m_World, selected, inspector);
					InspectComponent<eng::camera::ProjectionComponent>(m_World, selected, inspector);
					InspectComponent<eng::camera::Zoom2DComponent>(m_World, selected, inspector);
					InspectComponent<eng::PhysicsComponent>(m_World, selected, inspector);
					InspectComponent<eng::PrototypeComponent>(m_World, selected, inspector);
					InspectComponent<eng::SpriteComponent>(m_World, selected, inspector);
					InspectComponent<hidden::GroupComponent>(m_World, selected, inspector);
					InspectComponent<hidden::ObjectComponent>(m_World, selected, inspector);
					inspector.End();
				}
			}
		}
		ImGui::End();

		// save pop-up
		{
			constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
			constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
			constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

			if (world.HasComponent<editor::EntitySaveComponent>(windowEntity))
				world.RemoveComponent<editor::EntitySaveComponent>(windowEntity);

			if (HasInput(world, input::EKeyboard::S) || world.HasComponent<editor::EntitySaveComponent>(windowEntity))
			{
				const auto& readSettings = world.ReadSingleton<editor::settings::LocalComponent>();
				const auto& readWindow = world.ReadComponent<editor::EntityWindowComponent>(windowEntity);
				const ecs::Entity selected = windowComponent.m_Selected;

				const auto& readPrototype = m_World.ReadComponent<eng::PrototypeComponent>(selected);
				const auto& readName = m_World.ReadComponent<ecs::NameComponent>(selected);

				str::Path filepath = readPrototype.m_Path;
				if (filepath.IsEmpty())
				{
					eng::SaveFileSettings settings;
					settings.m_Title = "Save Sprite";
					settings.m_Filters = { "Prototypes (*.prototype)", "*.prototype" };
					settings.m_Path = str::Path(readSettings.m_Entity.m_Save, readName.m_Name, eng::PrototypeManager::s_Extension);
					filepath = eng::SaveFileDialog(settings);
				}

				if (!filepath.IsEmpty())
				{
					auto& writePrototype = m_World.WriteComponent<eng::PrototypeComponent>(selected);
					writePrototype.m_Path = filepath;
					auto& writeSettings = world.WriteSingleton<editor::settings::LocalComponent>();
					writeSettings.m_Entity.m_Save = filepath.GetDirectory();

					eng::Visitor visitor;
					visitor.Write("m_Guid", readPrototype.m_Guid);
					visitor.Write("m_Name", readName.m_Name);

					SaveComponent<eng::camera::Bound2DComponent>(m_World, selected, visitor);
					SaveComponent<eng::camera::Move2DComponent>(m_World, selected, visitor);
					SaveComponent<eng::camera::Move3DComponent>(m_World, selected, visitor);
					SaveComponent<eng::camera::Pan3DComponent>(m_World, selected, visitor);
					SaveComponent<eng::camera::ProjectionComponent>(m_World, selected, visitor);
					SaveComponent<eng::camera::Zoom2DComponent>(m_World, selected, visitor);
					SaveComponent<eng::PhysicsComponent>(m_World, selected, visitor);
					SaveComponent<eng::SpriteComponent>(m_World, selected, visitor);
					SaveComponent<eng::TransformComponent>(m_World, selected, visitor);
					SaveComponent<hidden::GroupComponent>(m_World, selected, visitor);
					SaveComponent<hidden::ObjectComponent>(m_World, selected, visitor);

					str::String string = visitor;
					visitor.SaveToFile(filepath);
				}
			}
		}

		if (!isOpen)
			world.DestroyEntity(windowEntity);
	}
}