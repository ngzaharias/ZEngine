#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"

namespace eng
{
	struct LinesComponent;
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

namespace editor
{
	class GizmoSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::LinesComponent,
			eng::TransformComponent,
			const editor::EntityWindowComponent,
			const editor::settings::LocalComponent,
			const eng::camera::ProjectionComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
