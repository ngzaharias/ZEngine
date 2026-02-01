#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::hidden
{
	struct ObjectComponent;
	struct RevealComponent;
}

namespace eng
{
	struct PhysicsComponent;
}

namespace client::hidden
{
	class PhysicsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::PhysicsComponent>
			::Read<
			client::hidden::ObjectComponent,
			client::hidden::RevealComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}