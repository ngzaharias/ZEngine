#pragma once

#include "ECS/System.h"

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
		using World = ecs::WorldView<
			eng::SavegameComponent,
			hidden::RevealComponent,
			const eng::PrototypeComponent,
			const hidden::GroupComponent,
			const hidden::ObjectComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}