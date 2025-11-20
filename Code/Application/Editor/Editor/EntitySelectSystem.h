#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"

namespace editor
{
	struct EntitySelectComponent;
}

namespace eng
{
	class AssetManager;
	class InputManager;
	class WindowManager;
	struct LinesComponent;
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
	struct DebugComponent;
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
			editor::EntitySelectComponent,
			eng::LinesComponent,
			const eng::camera::EditorComponent,
			const eng::camera::ProjectionComponent,
			const eng::SpriteComponent,
			const eng::TransformComponent,
			const eng::VisibilityComponent,
			const eng::settings::DebugComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}