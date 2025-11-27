#pragma once

#include "ECS/System.h"

namespace dbg::hidden
{
	struct ObjectComponent;
}

namespace eng
{
	struct LinesSingleton;
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
	struct DebugSingleton;
}

namespace dbg::hidden
{
	class ObjectSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Components
			dbg::hidden::ObjectComponent,
			eng::level::EntityComponent,
			eng::SpriteComponent,
			eng::TransformComponent,
			const ::hidden::ObjectComponent,
			// Singletons
			eng::LinesSingleton,
			const ::hidden::settings::DebugSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}