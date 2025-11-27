#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"

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
		using World = ecs::WorldView<
			// Resources
			eng::InputManager,
			const eng::AssetManager,
			const eng::WindowManager,
			// Components
			const eng::camera::EditorComponent,
			const eng::camera::ProjectionComponent,
			const eng::SpriteComponent,
			const eng::TransformComponent,
			const eng::VisibilityComponent,
			// Singletons
			editor::EntitySelectSingleton,
			eng::LinesSingleton,
			const eng::settings::DebugSingleton>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}