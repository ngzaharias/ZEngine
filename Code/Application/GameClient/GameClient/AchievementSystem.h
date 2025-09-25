#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"

namespace eng
{
	class AchievementTable;
	class InputManager;
	class PlatformManager;
	class WindowManager;
	struct PhysicsSceneComponent;
	struct PrototypeComponent;
	struct TransformComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace hidden
{
	struct GroupComponent;
	struct RevealComponent;
}

namespace client
{
	class AchievementSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// resources
			eng::InputManager,
			eng::PlatformManager,
			const eng::AchievementTable,
			const eng::WindowManager,
			// components
			const eng::camera::ProjectionComponent,
			const eng::PhysicsSceneComponent,
			const eng::PrototypeComponent,
			const eng::TransformComponent,
			const hidden::GroupComponent,
			const hidden::RevealComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}