#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace debug::hidden
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

namespace debug::hidden
{
	class ObjectSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			debug::hidden::ObjectComponent,
			eng::level::EntityComponent,
			eng::LinesSingleton,
			eng::SpriteComponent,
			eng::TransformComponent>
			::Read<
			::hidden::ObjectComponent,
			::hidden::settings::DebugSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}