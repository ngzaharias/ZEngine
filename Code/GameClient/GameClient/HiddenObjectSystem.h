#pragma once

#include <ECS/System.h>

namespace eng
{
	struct CameraComponent;
	struct InputComponent;
	struct LinesComponent;
	struct PhysicsSceneComponent;
	struct RigidStaticComponent;
	struct SpriteComponent;
	struct TransformComponent;
}

namespace hidden
{
	struct ObjectComponent;

	class ObjectSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::LinesComponent,
			eng::RigidStaticComponent,
			eng::SpriteComponent,
			const eng::CameraComponent,
			const eng::InputComponent,
			const eng::PhysicsSceneComponent,
			const eng::TransformComponent,
			const hidden::ObjectComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}