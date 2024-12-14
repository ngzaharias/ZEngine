#pragma once

#include "ECS/System.h"

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
		using World = ecs::WorldView<
			eng::SpriteComponent,
			const eng::PrototypeComponent,
			const hidden::GroupComponent,
			const hidden::RevealComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}