#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::hidden
{
	struct GroupComponent;
	struct RevealComponent;
}

namespace eng
{
	struct PrototypeComponent;
	struct SpriteComponent;
}

namespace client::hidden
{
	class TrackerSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::SpriteComponent>
			::Read<
			client::hidden::GroupComponent,
			client::hidden::RevealComponent,
			eng::PrototypeComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}