#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"

namespace clt::settings
{
	struct DebugComponent;
}

namespace eng
{
	class WindowManager;
	struct LinesComponent;
	struct InputComponent;
	struct PhysicsSceneComponent;
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
	struct DebugComponent;
}

namespace hidden
{
	class RevealSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView <
			// Resources
			const eng::WindowManager,
			// Components
			eng::LinesComponent,
			eng::RigidStaticComponent,
			hidden::RevealComponent,
			const eng::camera::ProjectionComponent,
			const eng::InputComponent,
			const eng::PhysicsSceneComponent,
			const eng::TransformComponent,
			const hidden::ObjectComponent,
			const hidden::settings::DebugComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}