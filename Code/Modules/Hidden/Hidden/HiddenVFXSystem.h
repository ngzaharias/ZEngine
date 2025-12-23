#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace ecs
{
	struct NameComponent;
}

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
	struct VFXComponent;
}

namespace hidden
{
	class VFXSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			eng::FlipbookComponent,
			eng::level::EntityComponent,
			eng::TransformComponent,
			hidden::VFXComponent>
			::Read<
			hidden::RevealComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}