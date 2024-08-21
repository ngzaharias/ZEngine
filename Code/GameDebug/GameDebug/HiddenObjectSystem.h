#pragma once

#include "ECS/System.h"

namespace eng
{
	struct LinesComponent;
	struct TransformComponent;
}

namespace hidden
{
	struct ObjectComponent;
}

namespace dbg::hidden
{
	class ObjectSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::LinesComponent,
			const eng::TransformComponent,
			const ::hidden::ObjectComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}