#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"

namespace eng
{
	struct LinesComponent;
}

namespace editor::settings
{
	struct LocalComponent;
}

namespace editor
{
	class GizmoAxes final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::LinesComponent,
			const editor::settings::LocalComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
