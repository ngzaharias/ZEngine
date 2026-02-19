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
	struct OpenWindowEvent;
	struct SaveComponent;
	struct SelectSingleton;
	struct SettingsSingleton;
	struct WindowComponent;
}

namespace eng
{
	class AssetManager;
	class InputManager;
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
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::entity::SaveComponent,
			editor::entity::SelectSingleton,
			editor::entity::SettingsSingleton,
			editor::entity::WindowComponent>
			::Read<
			editor::entity::OpenWindowEvent,
			eng::AssetManager,
			eng::InputManager,
			eng::level::EntityComponent,
			eng::PrototypeComponent,
			gamestate::EditorComponent>;

		WindowSystem(ecs::EntityWorld& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_World;
		imgui::Identifier m_WindowIds = {};
	};
}
