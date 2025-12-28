#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::settings
{
	struct DebugComponent;
}

namespace eng
{
	class InputManager;
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct LinesSingleton;
	struct PhysicsSceneSingleton;
	struct RigidStaticComponent;
	struct TransformComponent;
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
		using World = ecs::WorldView
			::Write<
			eng::LinesSingleton,
			eng::RigidStaticComponent,
			hidden::RevealComponent>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::InputManager,
			eng::PhysicsSceneSingleton,
			eng::TransformComponent,
			eng::WindowManager,
			hidden::ObjectComponent,
			hidden::settings::DebugSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}