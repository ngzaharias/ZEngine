#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct PrototypeComponent;
	struct SavegameComponent;
}

namespace hidden
{
	struct GroupComponent;
	struct RevealComponent;
	struct ObjectComponent;
}

namespace hidden
{
	class SaveLoadSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::SavegameComponent,
			hidden::RevealComponent>
			::Read<
			eng::PrototypeComponent,
			hidden::GroupComponent,
			hidden::ObjectComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}