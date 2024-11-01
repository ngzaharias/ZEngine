#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"

namespace eng
{
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

namespace hidden
{
	class RevealSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::RigidStaticComponent,
			hidden::RevealComponent,
			const eng::camera::ProjectionComponent,
			const eng::InputComponent,
			const eng::PhysicsSceneComponent,
			const eng::TransformComponent,
			const hidden::ObjectComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}