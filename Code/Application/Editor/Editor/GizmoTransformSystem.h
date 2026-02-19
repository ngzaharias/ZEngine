#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::gizmo
{
	struct TransformSingleton;
}

namespace editor::outliner
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

namespace gamestate
{
	struct EditModeComponent;
}

namespace editor::gizmo
{
	class TransformSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::gizmo::TransformSingleton,
			eng::PhysicsComponent,
			eng::TransformComponent>
			::Read<
			editor::outliner::SelectSingleton,
			editor::settings::LocalSingleton,
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::InputManager,
			eng::WindowManager,
			gamestate::EditModeComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
