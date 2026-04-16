#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::hidden
{
	struct GroupComponent;
	struct GroupTemplate;
	struct RevealComponent;
}

namespace eng
{
	struct UUIDComponent;
}

namespace client::hidden
{
	class GroupSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			client::hidden::GroupComponent>
			::Read<
			client::hidden::GroupTemplate,
			client::hidden::RevealComponent,
			eng::UUIDComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}