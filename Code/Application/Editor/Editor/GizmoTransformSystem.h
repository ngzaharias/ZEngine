#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::gizmo
{
	struct TransformComponent;
}

namespace editor::outliner
{
	struct SelectComponent;
}

namespace editor::settings
{
	struct LocalComponent;
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
			editor::gizmo::TransformComponent,
			eng::InputManager,
			eng::PhysicsComponent,
			eng::TransformComponent>
			::Read<
			editor::outliner::SelectComponent,
			editor::settings::LocalComponent,
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::WindowManager,
			gamestate::EditModeComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
