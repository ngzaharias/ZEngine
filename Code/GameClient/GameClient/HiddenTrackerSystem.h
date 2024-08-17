#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"

namespace gui::modal
{
	struct MessageComponent;
}

namespace hidden
{
	struct ObjectComponent;
	struct RevealedComponent;
}

namespace hidden
{
	class TrackerSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			gui::modal::MessageComponent,
			const hidden::ObjectComponent,
			const hidden::RevealedComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}