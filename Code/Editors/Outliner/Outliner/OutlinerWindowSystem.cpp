#include "OutlinerPCH.h"
#include "Outliner/OutlinerWindowSystem.h"

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
#include "Icons/Icons.h"
#include "Outliner/OutlinerOpenWindowEvent.h"
#include "Outliner/OutlinerSelectSingleton.h"
#include "Outliner/OutlinerWindowComponent.h"

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
	const str::Name strInput = str::Name::Create("Outliner");
	const str::Name strSave = str::Name::Create("Outliner_Save");
	const str::Name strSprite = str::Name::Create("Sprite");

	using World = editor::outliner::WindowSystem::World;
	using WindowView = ecs::EntityView
		::Required<
		editor::outliner::WindowComponent>;

	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}

	ecs::Entity CreateEntity(ecs::EntityWorld& world, const str::StringView name, const str::Name& level)
	{
		const ecs::Entity entity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(entity, name);
		world.AddComponent<eng::level::EntityComponent>(entity, level);
		world.AddComponent<eng::TransformComponent>(entity);

		auto& prototypeComponent = world.AddComponent<eng::PrototypeComponent>(entity);
		prototypeComponent.m_Guid = str::Guid::Generate();

		auto& select = world.WriteSingleton<editor::outliner::SelectSingleton>();
		select.m_Entity = entity;

		return entity;
	}

	bool ButtonIcon(const char* label, const char* tooltip, const icon::Data& icon, const ImVec2& size = ImVec2(22, 22), const bool active = false)
	{
		const bool result = imgui::ImageButton(label, active, icon.m_TextureId, size, icon.m_UV0, icon.m_UV1);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
		ImGui::SetItemTooltip(tooltip);
		ImGui::PopStyleVar();
		return result;
	}

	void Draw_EntityList(World& world)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.f, 0.f));
		ButtonIcon("##entity", "Entity", icon::ENTITY);
		ImGui::SameLine();
		ButtonIcon("##camera", "Camera", icon::ENTITY_CAMERA);
		ImGui::SameLine();
		ButtonIcon("##flipbook", "Flipbook", icon::ENTITY_FLIPBOOK);
		ImGui::SameLine();
		ButtonIcon("##light", "Light", icon::ENTITY_LIGHT);
		ImGui::SameLine();
		ButtonIcon("##sound", "Sound", icon::ENTITY_SOUND);
		ImGui::SameLine();
		ButtonIcon("##sprite", "Sprite", icon::ENTITY_SPRITE);
		ImGui::SameLine();
		ButtonIcon("##static mesh", "Static Mesh", icon::ENTITY_STATICMESH);
		ImGui::PopStyleVar();
		ImGui::Spacing();
		ImGui::Separator();

		const auto& select = world.ReadSingleton<editor::outliner::SelectSingleton>();

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
				auto& select = world.WriteSingleton<editor::outliner::SelectSingleton>();
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
}

editor::outliner::WindowSystem::WindowSystem(ecs::EntityWorld& world)
	: m_World(world)
{
}

void editor::outliner::WindowSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!world.HasAny<ecs::query::Include<gamestate::EditorComponent>>())
		return;

	for (const auto& request : world.Events<editor::outliner::OpenWindowEvent>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Outliner");

		auto& window = world.AddComponent<editor::outliner::WindowComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_Label = ToLabel("Outliner##outliner", identifier);
	}

	for (auto&& view : world.Query<ecs::query::Removed<const editor::outliner::WindowComponent>>())
	{
		const auto& window = world.ReadComponent<editor::outliner::WindowComponent>(view, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (auto&& windowView : world.Query<ecs::query::Include<editor::outliner::WindowComponent>>())
	{
		constexpr ImGuiWindowFlags s_WindowFlags =
			ImGuiWindowFlags_NoCollapse;

		constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
		constexpr Vector2f s_DefaultSize = Vector2f(800, 600.f);

		const auto& window = windowView.WriteRequired<editor::outliner::WindowComponent>();

		bool isOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(window.m_Label.c_str(), &isOpen, s_WindowFlags))
		{
			str::Name levelName = {};
			for (auto&& view : world.Query<ecs::query::Include<const eng::level::EntityComponent>>())
			{
				const auto& level = view.ReadRequired<eng::level::EntityComponent>();
				levelName = level.m_Name;
			}

			Draw_EntityList(world);
			Draw_EntityDrop(m_World, levelName);
		}
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(windowView);
	}
}