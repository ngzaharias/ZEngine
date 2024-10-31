#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"

namespace eng
{
	struct FlipbookComponent;
	struct TransformComponent;
}

namespace eng::level
{
	struct EntityComponent;
}

namespace hidden
{
	struct RevealComponent;
}

namespace hidden
{
	class VFXSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::FlipbookComponent,
			eng::level::EntityComponent,
			eng::TransformComponent,
			const hidden::RevealComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}