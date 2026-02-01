#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::hidden
{
	struct RevealComponent;
	struct VFXComponent;
}

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

namespace client::hidden
{
	class VFXSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			client::hidden::VFXComponent,
			ecs::NameComponent,
			eng::FlipbookComponent,
			eng::level::EntityComponent,
			eng::TransformComponent>
			::Read<
			client::hidden::RevealComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}