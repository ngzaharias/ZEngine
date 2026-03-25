#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor::inspector
{
	struct OpenWindowEvent;
	struct SaveComponent;
	struct SettingsComponent;
	struct WindowComponent;
}

namespace editor::outliner
{
	struct SelectComponent;
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

namespace editor::inspector
{
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::inspector::SaveComponent,
			editor::inspector::SettingsComponent,
			editor::inspector::WindowComponent,
			editor::outliner::SelectComponent,
			eng::InputManager>
			::Read<
			editor::inspector::OpenWindowEvent,
			eng::AssetManager,
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
