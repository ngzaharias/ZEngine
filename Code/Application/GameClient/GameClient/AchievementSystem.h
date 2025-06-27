#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"

namespace eng
{
	class AchievementTable;
	class PlatformManager;
	struct PrototypeComponent;
}

namespace hidden
{
	struct RevealComponent;
}

namespace clt
{
	class AchievementSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// resources
			eng::PlatformManager,
			const eng::AchievementTable,
			// components
			const eng::PrototypeComponent,
			const hidden::RevealComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}