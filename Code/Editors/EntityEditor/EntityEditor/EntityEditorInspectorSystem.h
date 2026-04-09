#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor::entity
{
	class CommandManager;
	struct HistoryComponent;
	struct InspectorComponent;
	struct OpenInspectorEvent;
	struct SelectComponent;
	struct SettingsComponent;
}

namespace eng
{
	class AssetManager;
	class InputManager;
	class TemplateManager;
	struct CameraComponent;
	struct PhysicsComponent;
	struct PrototypeComponent;
	struct SpriteComponent;
	struct TransformComponent;
}

namespace eng::level
{
	struct EntityComponent;
}

namespace gamestate
{
	struct EditorComponent;
}

namespace editor::entity
{
	class InspectorSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::entity::CommandManager,
			editor::entity::InspectorComponent,
			editor::entity::SelectComponent,
			editor::entity::SettingsComponent,
			eng::InputManager,
			eng::TemplateManager>
			::Read<
			editor::entity::HistoryComponent,
			editor::entity::OpenInspectorEvent,
			eng::AssetManager,
			eng::level::EntityComponent,
			eng::PrototypeComponent,
			gamestate::EditorComponent>;

		InspectorSystem(ecs::EntityWorld& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_World;
		imgui::Identifier m_WindowIds = {};
	};
}
