#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"

namespace editor
{
	struct EntitySelectSingleton;
}

namespace editor::settings
{
	struct LocalSingleton;
}

namespace eng
{
	class InputManager;
	class WindowManager;
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
	struct DebugSingleton;
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
			const eng::camera::EditorComponent,
			const eng::camera::ProjectionComponent,
			const eng::settings::DebugSingleton,
			// Singletons
			const editor::EntitySelectSingleton,
			const editor::settings::LocalSingleton>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
