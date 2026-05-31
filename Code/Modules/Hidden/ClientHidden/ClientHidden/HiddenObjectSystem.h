#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::hidden
{
	struct ObjectComponent;
	struct ObjectTemplate;
}

namespace client::hidden
{
	class ObjectSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			client::hidden::ObjectComponent>
			::Read<
			client::hidden::ObjectTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
}