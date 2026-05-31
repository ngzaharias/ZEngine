#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::hidden
{
	struct DebugComponent;
	struct ObjectComponent;
	struct RevealComponent;
}

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
	struct LinesComponent;
	struct PhysicsSceneComponent;
	struct RigidStaticComponent;
	struct TransformComponent;
	struct UUIDComponent;
}

namespace shared::hidden
{
	struct RevealedEvent;
	struct SelectedEvent;
}

namespace client::hidden
{
	class RevealSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			client::hidden::RevealComponent,
			eng::InputManager,
			eng::LinesComponent,
			eng::RigidStaticComponent,
			shared::hidden::SelectedEvent>
			::Read<
			client::hidden::DebugComponent,
			client::hidden::ObjectComponent,
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::PhysicsSceneComponent,
			eng::TransformComponent,
			eng::UUIDComponent,
			eng::WindowManager,
			shared::hidden::RevealedEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}