#include "InspectorPCH.h"
#include "Inspector/InspectorWindowSystem.h"

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
#include "GameState/GameStateEditorComponent.h"
#include "Inspector/InspectorOpenWindowEvent.h"
#include "Inspector/InspectorSaveComponent.h"
#include "Inspector/InspectorSettingsSingleton.h"
#include "Inspector/InspectorWindowComponent.h"
#include "Outliner/OutlinerSelectSingleton.h"

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

	using World = editor::inspector::WindowSystem::World;
	using WindowView = ecs::EntityView
		::Required<
		editor::inspector::WindowComponent>
		::Optional<
		editor::inspector::SaveComponent>;

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

	void Draw_Inspector(ecs::EntityWorld& world, const WindowView& view)
	{
		const auto& select = world.ReadSingleton<editor::outliner::SelectSingleton>();
		const auto& window = view.ReadRequired<editor::inspector::WindowComponent>();

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

	void Draw_SavePopup(ecs::EntityWorld& world, const WindowView& view)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		if (view.HasOptional<editor::inspector::SaveComponent>())
			world.RemoveComponent<editor::inspector::SaveComponent>(view);

		const auto& input = world.ReadResource<eng::InputManager>();
		if (input.IsPressed(strSave) || view.HasOptional<editor::inspector::SaveComponent>())
		{
			const auto& select = world.ReadSingleton<editor::outliner::SelectSingleton>();
			const auto& settings = world.ReadSingleton<editor::inspector::SettingsSingleton>();

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
					auto& settings = world.WriteSingleton<editor::inspector::SettingsSingleton>();
					settings.m_Save = filepath.GetDirectory();

					auto& manager = world.WriteResource<eng::PrototypeManager>();
					manager.SaveEntity(world, selected, filepath);
				}
			}
		}
	}
}

editor::inspector::WindowSystem::WindowSystem(ecs::EntityWorld& world)
	: m_World(world)
{
}

void editor::inspector::WindowSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!world.HasAny<ecs::query::Include<gamestate::EditorComponent>>())
		return;

	if (world.HasAny<ecs::query::Added<editor::inspector::WindowComponent>>())
	{
		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::EditorUI;
		layer.m_Bindings.Emplace(strSave, input::EKey::S, input::EKey::Control_L);
		layer.m_Bindings.Emplace(strSave, input::EKey::S, input::EKey::Control_R);

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInput, layer);
	}

	if (world.HasAny<ecs::query::Removed<editor::inspector::WindowComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	for (const auto& request : world.Events<editor::inspector::OpenWindowEvent>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Inspector");

		auto& window = world.AddComponent<editor::inspector::WindowComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_Label = ToLabel("Inspector##inspector", identifier);
	}

	for (auto&& view : world.Query<ecs::query::Removed<const editor::inspector::WindowComponent>>())
	{
		const auto& window = world.ReadComponent<editor::inspector::WindowComponent>(view, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (auto&& windowView : world.Query<ecs::query::Include<editor::inspector::WindowComponent>::Optional<editor::inspector::SaveComponent>>())
	{
		constexpr ImGuiWindowFlags s_WindowFlags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_MenuBar;

		constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
		constexpr Vector2f s_DefaultSize = Vector2f(800, 600.f);

		const auto& window = windowView.WriteRequired<editor::inspector::WindowComponent>();

		bool isOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(window.m_Label.c_str(), &isOpen, s_WindowFlags))
		{
			Draw_Inspector(m_World, windowView);
			Draw_SavePopup(m_World, windowView);
		}
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(windowView);
	}
}