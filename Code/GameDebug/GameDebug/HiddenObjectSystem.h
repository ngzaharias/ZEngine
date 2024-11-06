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
	struct SpriteRequestComponent;
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
	class ObjectSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			dbg::hidden::ObjectComponent,
			eng::LinesComponent,
			eng::level::EntityComponent,
			eng::SpriteComponent,
			eng::SpriteRequestComponent,
			eng::TransformComponent,
			const eng::settings::DebugComponent,
			const ::hidden::ObjectComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}