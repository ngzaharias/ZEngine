#pragma once

#include "ECS/System.h"

namespace eng
{
	struct LinesComponent;
	struct SpriteComponent;
	struct TransformComponent;
}

namespace eng::level
{
	struct EntityComponent;
}

namespace eng::settings
{
	struct DebugComponent;
}

namespace hidden
{
	struct ObjectComponent;
}

namespace dbg::hidden
{
	struct DebugComponent;

	class ObjectSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			dbg::hidden::DebugComponent,
			eng::LinesComponent,
			eng::level::EntityComponent,
			eng::SpriteComponent,
			eng::TransformComponent,
			const eng::settings::DebugComponent,
			const ::hidden::ObjectComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}