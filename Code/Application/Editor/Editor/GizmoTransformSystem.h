#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::entity
{
	class CommandManager;
	struct SelectComponent;
}

namespace editor::gizmo
{
	struct TransformComponent;
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
	struct PhysicsTemplate;
	struct TransformComponent;
	struct TransformTemplate;
	struct UUIDComponent;
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
			editor::entity::CommandManager,
			editor::gizmo::TransformComponent,
			eng::InputManager,
			eng::PhysicsTemplate>
			::Read<
			editor::entity::SelectComponent,
			editor::settings::LocalComponent,
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::TransformComponent,
			eng::TransformTemplate,
			eng::UUIDComponent,
			eng::WindowManager,
			gamestate::EditModeComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
