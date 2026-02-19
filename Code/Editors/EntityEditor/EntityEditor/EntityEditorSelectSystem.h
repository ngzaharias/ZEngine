#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::entity
{
	struct SelectSingleton;
}

namespace eng
{
	class AssetManager;
	class InputManager;
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct EditorComponent;
	struct LinesSingleton;
	struct SpriteComponent;
	struct TransformComponent;
	struct VisibilityComponent;
}

namespace gamestate
{
	struct EditModeComponent;
}

namespace editor::entity
{
	class SelectSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::entity::SelectSingleton,
			eng::InputManager,
			eng::LinesSingleton>
			::Read<
			eng::ActiveComponent,
			eng::AssetManager,
			eng::CameraComponent,
			eng::EditorComponent,
			eng::SpriteComponent,
			eng::TransformComponent,
			eng::VisibilityComponent,
			eng::WindowManager,
			gamestate::EditModeComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}