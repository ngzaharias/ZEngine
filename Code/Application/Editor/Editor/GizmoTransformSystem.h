#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::entity
{
	struct SelectSingleton;
}

namespace editor::settings
{
	struct LocalSingleton;
}

namespace eng
{
	class InputManager;
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct PhysicsComponent;
	struct TransformComponent;
	struct VersionComponent;
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
		using World = ecs::WorldView
			::Write<
			eng::PhysicsComponent,
			eng::TransformComponent>
			::Read<
			editor::entity::SelectSingleton,
			editor::settings::LocalSingleton,
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::InputManager,
			eng::settings::DebugSingleton,
			eng::WindowManager>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
