#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"

namespace eng
{
	struct SpriteComponent;
}

namespace hidden
{
	struct ObjectComponent;
	struct RevealComponent;
}

namespace hidden::settings
{
	struct LocalComponent;
}

namespace hidden
{
	class SpriteSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::SpriteComponent,
			const hidden::ObjectComponent,
			const hidden::RevealComponent,
			const hidden::settings::LocalComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}