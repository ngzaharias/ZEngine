#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"

namespace eng
{
	struct LinesComponent;
	struct TransformComponent;
}

namespace eng::camera
{
	struct  ProjectionComponent;
}

namespace editor::settings
{
	struct LocalComponent;
}

namespace editor::gizmo
{
	class CrosshairSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::LinesComponent,
			const editor::settings::LocalComponent,
			const eng::camera::ProjectionComponent,
			const eng::TransformComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
