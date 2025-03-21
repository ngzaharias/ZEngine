#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"

namespace eng
{
	class WindowManager;
	struct InputComponent;
	struct LinesComponent;
	struct PhysicsComponent;
	struct TransformComponent;
	struct VersionComponent;
}

namespace eng::camera
{
	struct  ProjectionComponent;
}

namespace editor
{
	struct EntityWindowComponent;
}

namespace editor::settings
{
	struct LocalComponent;
}

namespace editor::gizmo
{
	class TransformSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::WindowManager,
			// Components
			eng::PhysicsComponent,
			eng::TransformComponent,
			const editor::EntityWindowComponent,
			const editor::settings::LocalComponent,
			const eng::camera::ProjectionComponent,
			const eng::InputComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
