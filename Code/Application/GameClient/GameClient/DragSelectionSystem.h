#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace drag
{
	struct IsSelectableComponent;
	struct SelectionComponent;
}

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class InputManager;
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct LinesSingleton;
	struct PhysicsSceneSingleton;
	struct TransformComponent;
}

namespace drag
{
	class SelectionSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			drag::SelectionComponent,
			ecs::NameComponent,
			eng::LinesSingleton>
			::Read<
			drag::IsSelectableComponent,
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::InputManager,
			eng::PhysicsSceneSingleton,
			eng::TransformComponent,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}