#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct PhysicsComponent;
}

namespace hidden
{
	struct ObjectComponent;
	struct RevealComponent;
}

namespace hidden
{
	class PhysicsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::PhysicsComponent>
			::Read<
			hidden::ObjectComponent,
			hidden::RevealComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}