#pragma once

#include <ECS/Entity.h>
#include <ECS/System.h>

namespace eng
{
	struct SpriteComponent;
}

namespace hidden
{
	struct ObjectComponent;
	struct RevealedComponent;
}

namespace hidden
{
	class SpriteSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::SpriteComponent,
			const hidden::ObjectComponent,
			const hidden::RevealedComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}