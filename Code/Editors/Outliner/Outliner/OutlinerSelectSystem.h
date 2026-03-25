#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::outliner
{
	struct SelectComponent;
}

namespace eng
{
	class AssetManager;
	class InputManager;
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct EditorComponent;
	struct LinesComponent;
	struct SpriteComponent;
	struct TransformComponent;
	struct VisibilityComponent;
}

namespace eng::level
{
	struct EntityComponent;
}

namespace gamestate
{
	struct EditModeComponent;
	struct EditorComponent;
}

namespace editor::outliner
{
	class SelectSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::outliner::SelectComponent,
			eng::InputManager,
			eng::LinesComponent>
			::Read<
			eng::ActiveComponent,
			eng::AssetManager,
			eng::CameraComponent,
			eng::EditorComponent,
			eng::level::EntityComponent,
			eng::SpriteComponent,
			eng::TransformComponent,
			eng::VisibilityComponent,
			eng::WindowManager,
			gamestate::EditModeComponent,
			gamestate::EditorComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}