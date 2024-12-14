#pragma once

#include "ECS/System.h"

namespace dbg::hidden
{
	struct ObjectComponent;
}

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

namespace hidden
{
	struct ObjectComponent;
}

namespace hidden::settings
{
	struct DebugComponent;
}

namespace dbg::hidden
{
	class ObjectSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			dbg::hidden::ObjectComponent,
			eng::LinesComponent,
			eng::level::EntityComponent,
			eng::SpriteComponent,
			eng::TransformComponent,
			const ::hidden::ObjectComponent,
			const ::hidden::settings::DebugComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}