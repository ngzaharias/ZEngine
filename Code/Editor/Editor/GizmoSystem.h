#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"

namespace eng
{
	struct LinesComponent;
}

namespace editor
{
	class GizmoSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::LinesComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
