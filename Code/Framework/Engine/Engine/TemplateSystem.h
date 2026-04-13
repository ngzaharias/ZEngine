#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct UUIDComponent;
	struct UUIDMapComponent;
}

namespace eng
{
	class TemplateSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::UUIDMapComponent>
			::Read<
			eng::UUIDComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}