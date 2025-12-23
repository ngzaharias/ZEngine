#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct PrototypeComponent;
	struct SpriteComponent;
}

namespace hidden
{
	struct GroupComponent;
	struct RevealComponent;
}

namespace hidden
{
	class TrackerSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::SpriteComponent>
			::Read<
			eng::PrototypeComponent,
			hidden::GroupComponent,
			hidden::RevealComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}