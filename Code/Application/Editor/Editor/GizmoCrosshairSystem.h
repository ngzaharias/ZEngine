#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	struct LinesSingleton;
	struct TransformComponent;
}

namespace eng::camera
{
	struct  ProjectionComponent;
}

namespace editor::settings
{
	struct LocalSingleton;
}

namespace editor::gizmo
{
	class CrosshairSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Components
			const eng::camera::ProjectionComponent,
			const eng::TransformComponent,
			// Singletons
			eng::LinesSingleton,
			const editor::settings::LocalSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}
