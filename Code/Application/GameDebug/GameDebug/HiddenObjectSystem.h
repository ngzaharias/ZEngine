#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::hidden
{
	struct DebugComponent;
	struct ObjectComponent;
}

namespace debug::hidden
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

namespace debug::hidden
{
	class ObjectSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			debug::hidden::ObjectComponent,
			eng::level::EntityComponent,
			eng::LinesComponent,
			eng::SpriteComponent,
			eng::TransformComponent>
			::Read<
			client::hidden::DebugComponent,
			client::hidden::ObjectComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}