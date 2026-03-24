#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::hidden
{
	struct DebugStaticComponent;
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
	struct LinesStaticComponent;
	struct PhysicsSceneStaticComponent;
	struct PrototypeComponent;
	struct RigidStaticComponent;
	struct TransformComponent;
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
			eng::LinesStaticComponent,
			eng::RigidStaticComponent,
			shared::hidden::SelectedEvent>
			::Read<
			client::hidden::DebugStaticComponent,
			client::hidden::ObjectComponent,
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::PhysicsSceneStaticComponent,
			eng::PrototypeComponent,
			eng::TransformComponent,
			eng::WindowManager,
			shared::hidden::RevealedEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}