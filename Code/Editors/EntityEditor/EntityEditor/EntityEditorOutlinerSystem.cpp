#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorOutlinerSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraTemplate.h"
#include "Engine/FileHelpers.h"
#include "Engine/FlipbookTemplate.h"
#include "Engine/InputManager.h"
#include "Engine/InspectorHelpers.h"
#include "Engine/LightAmbientTemplate.h"
#include "Engine/LightDirectionalTemplate.h"
#include "Engine/LightPointTemplate.h"
#include "Engine/PhysicsTemplate.h"
#include "Engine/SpriteTemplate.h"
#include "Engine/StaticMeshTemplate.h"
#include "Engine/TemplateManager.h"
#include "Engine/TransformTemplate.h"
#include "Engine/UUIDHelpers.h"
#include "Engine/VisibilityTemplate.h"
#include "EntityEditor/EntityEditorCommandManager.h"
#include "EntityEditor/EntityEditorOpenOutlinerEvent.h"
#include "EntityEditor/EntityEditorOutlinerComponent.h"
#include "EntityEditor/EntityEditorSelectComponent.h"
#include "GameState/GameStateEditorComponent.h"
#include "Icons/Icons.h"
#include "Serialize/Visitor.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_user.h"

template<>
bool imgui::WriteCustom(ecs::NameComponent& value)
{
	return Write("m_Name", value.m_Name);
}

namespace
{
	const str::Name strInput = str::Name::Create("Outliner");
	const str::Name strSave = str::Name::Create("Outliner_Save");
	const str::Name strSprite = str::Name::Create("Sprite");

	using World = editor::entity::OutlinerSystem::World;
	using WindowView = ecs::EntityView
		::Required<
		editor::entity::OutlinerComponent>;

	str::String ToLabel(const char* label, const int32 index)
	{
		return std::format("{}: {}", label, index);
	}

	using IconView = ecs::EntityView
		::Optional<
		eng::CameraTemplate,
		eng::FlipbookTemplate,
		eng::light::AmbientTemplate,
		eng::light::DirectionalTemplate,
		eng::light::PointTemplate,
		eng::SpriteTemplate,
		eng::StaticMeshTemplate>;
	const icon::Data& GetIcon(const IconView& view)
	{
		if (view.HasOptional<eng::CameraTemplate>())
			return icon::ENTITY_CAMERA;
		if (view.HasOptional<eng::FlipbookTemplate>())
			return icon::ENTITY_FLIPBOOK;
		if (view.HasOptional<eng::light::AmbientTemplate>())
			return icon::ENTITY_LIGHT;
		if (view.HasOptional<eng::light::DirectionalTemplate>())
			return icon::ENTITY_LIGHT;
		if (view.HasOptional<eng::light::PointTemplate>())
			return icon::ENTITY_LIGHT;
		if (view.HasOptional<eng::SpriteTemplate>())
			return icon::ENTITY_SPRITE;
		if (view.HasOptional<eng::StaticMeshTemplate>())
			return icon::ENTITY_STATICMESH;
		return icon::ENTITY;
	}

	void CreateEntity(World& world, const str::Path& path)
	{
		auto& commands = world.WriteResource<editor::entity::CommandManager>();
		commands.EntityCreate(path);
	}

	void DestroyEntity(World& world, const ecs::Entity& entity)
	{
		auto& commands = world.WriteResource<editor::entity::CommandManager>();
		commands.EntityDestroy(eng::ToUUID(world, entity));
	}

	void Icon(const icon::Data& icon, const ImVec2& size = ImVec2(15, 15))
	{
		imgui::Image(icon.m_TextureId, size, icon.m_UV0, icon.m_UV1);
	}

	bool ButtonIcon(const char* label, const char* tooltip, const icon::Data& icon, const ImVec2& size = ImVec2(22, 22), const bool active = false)
	{
		const bool result = imgui::ImageButton(label, active, icon.m_TextureId, size, icon.m_UV0, icon.m_UV1);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
		ImGui::SetItemTooltip(tooltip);
		ImGui::PopStyleVar();
		return result;
	}

	void Draw_MenuBar(World& world, const WindowView& view)
	{
		const auto& window = view.ReadRequired<editor::entity::OutlinerComponent>();
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Create"))
			{
				if (ImGui::MenuItem("Entity"))
					CreateEntity(world, {});
				ImGui::Separator();

				str::String name;
				for (const str::Path& filepath : window.m_Templates)
				{
					name = filepath.GetFileNameNoExtension();
					if (ImGui::MenuItem(name.c_str()))
						CreateEntity(world, filepath);
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void Draw_EntityList(ecs::EntityWorld& entityWorld, World& world)
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

		const auto& select = world.ReadComponent<editor::entity::SelectComponent>();

		using Query = ecs::query
			::Include<
			const eng::UUIDComponent>
			::Optional<
			const ecs::NameComponent,
			const eng::CameraTemplate,
			const eng::FlipbookTemplate,
			const eng::light::AmbientTemplate,
			const eng::light::DirectionalTemplate,
			const eng::light::PointTemplate,
			const eng::SpriteTemplate,
			const eng::StaticMeshTemplate>;
		for (auto&& view : world.Query<Query>())
		{
			imgui::RaiiID id(view.GetEntity().GetIndex());

			const char* name = "<unknown>";;
			if (const auto* component = view.ReadOptional<ecs::NameComponent>())
				name = component->m_Name.c_str();

			Icon(GetIcon(view));
			ImGui::SameLine();

			const bool isSelected = view == select.m_Entity;
			if (ImGui::Selectable(name, isSelected))
			{
				auto& select = world.WriteComponent<editor::entity::SelectComponent>();
				select.m_Entity = view;
			}

			ImGui::OpenPopupOnItemClick(name);
			if (ImGui::BeginPopup(name))
			{
				if (ImGui::Selectable("Destroy"))
					DestroyEntity(world, view);
				ImGui::EndPopup();
			}
		}
	}

	void Draw_EntityDrop(ecs::EntityWorld& world)
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
					//const ecs::Entity entity = CreateEntity(m_World, file.m_Name);
					//world.AddComponent<eng::VisibilityComponent>(entity);
					//auto& spriteComponent = world.AddComponent<eng::SpriteComponent>(entity);
					//spriteComponent.m_Sprite = file.m_Guid;
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
}

editor::entity::OutlinerSystem::OutlinerSystem(ecs::EntityWorld& world)
	: m_World(world)
{
}

void editor::entity::OutlinerSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!world.HasComponent<gamestate::EditorComponent>())
		return;

	for (const auto& request : world.Events<editor::entity::OpenOutlinerEvent>())
	{
		const int32 identifier = m_WindowIds.Borrow();
		const ecs::Entity windowEntity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(windowEntity, "Outliner");

		auto& window = world.AddComponent<editor::entity::OutlinerComponent>(windowEntity);
		window.m_Identifier = identifier;
		window.m_Label = ToLabel("Outliner", identifier);

		const str::Path directory = str::Path(str::EPath::Assets, "Templates");
		const eng::SearchDirectorySettings settings = { .m_Extension = ".template" };
		window.m_Templates = eng::SearchDirectory(directory, settings);
	}

	using RemovedQuery = ecs::query
		::Condition<ecs::Alive, ecs::Dead>
		::Removed<editor::entity::OutlinerComponent>;
	for (auto&& view : world.Query<RemovedQuery>())
	{
		const auto& window = world.ReadComponent<editor::entity::OutlinerComponent>(view, false);
		m_WindowIds.Release(window.m_Identifier);
	}

	for (auto&& view : world.Query<ecs::query::Include<editor::entity::OutlinerComponent>>())
	{
		constexpr ImGuiWindowFlags s_WindowFlags =
			ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoCollapse;

		constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
		constexpr Vector2f s_DefaultSize = Vector2f(800, 600.f);

		const auto& window = view.WriteRequired<editor::entity::OutlinerComponent>();

		bool isOpen = true;
		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(window.m_Label.c_str(), &isOpen, s_WindowFlags))
		{
			Draw_MenuBar(world, view);
			Draw_EntityList(m_World, world);
			Draw_EntityDrop(m_World);
		}
		ImGui::End();

		if (!isOpen)
			world.DestroyEntity(view);
	}
}