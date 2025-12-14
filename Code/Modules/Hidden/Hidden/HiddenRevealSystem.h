#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"

namespace client::settings
{
	struct DebugComponent;
}

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	class InputManager;
	class WindowManager;
	struct LinesSingleton;
	struct PhysicsSceneSingleton;
	struct RigidStaticComponent;
	struct TransformComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace hidden
{
	struct ObjectComponent;
	struct RevealComponent;
}

namespace hidden::settings
{
	struct DebugSingleton;
}

namespace hidden
{
	class RevealSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView <
			// Resources
			const eng::InputManager,
			const eng::WindowManager,
			// Components
			eng::RigidStaticComponent,
			hidden::RevealComponent,
			const eng::camera::ProjectionComponent,
			const eng::TransformComponent,
			const hidden::ObjectComponent,
			// Singletons
			eng::LinesSingleton,
			const eng::PhysicsSceneSingleton,
			const hidden::settings::DebugSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}