#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor
{
	struct EntitySelectSingleton;
}

namespace eng
{
	class AssetManager;
	class InputManager;
	class WindowManager;
	struct LinesSingleton;
	struct SpriteComponent;
	struct TransformComponent;
	struct VisibilityComponent;
}

namespace eng::camera
{
	struct EditorComponent;
	struct ProjectionComponent;
}

namespace eng::settings
{
	struct DebugSingleton;
}

namespace editor
{
	class EntitySelectSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::EntitySelectSingleton,
			eng::InputManager,
			eng::LinesSingleton>
			::Read<
			eng::AssetManager,
			eng::camera::EditorComponent,
			eng::camera::ProjectionComponent,
			eng::settings::DebugSingleton,
			eng::SpriteComponent,
			eng::TransformComponent,
			eng::VisibilityComponent,
			eng::WindowManager>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}