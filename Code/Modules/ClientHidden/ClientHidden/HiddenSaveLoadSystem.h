#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::hidden
{
	struct GroupComponent;
	struct RevealComponent;
	struct ObjectComponent;
}

namespace eng
{
	struct SavegameComponent;
	struct UUIDComponent;
}

namespace client::hidden
{
	class SaveLoadSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			client::hidden::RevealComponent,
			eng::SavegameComponent>
			::Read<
			client::hidden::GroupComponent,
			client::hidden::ObjectComponent,
			eng::UUIDComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}