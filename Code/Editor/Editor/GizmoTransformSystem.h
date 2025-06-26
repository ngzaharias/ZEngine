#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"

namespace editor
{
	struct EntitySelectComponent;
}

namespace editor::settings
{
	struct LocalComponent;
}

namespace eng
{
	class InputManager;
	class WindowManager;
	struct LinesComponent;
	struct PhysicsComponent;
	struct TransformComponent;
	struct VersionComponent;
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

namespace editor::gizmo
{
	class TransformSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::InputManager,
			const eng::WindowManager,
			// Components
			eng::PhysicsComponent,
			eng::TransformComponent,
			const editor::EntitySelectComponent,
			const editor::settings::LocalComponent,
			const eng::camera::EditorComponent,
			const eng::camera::ProjectionComponent,
			const eng::settings::DebugComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
