#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"

namespace eng
{
	struct PrototypeComponent;
	struct SpriteComponent;
}

namespace gui::modal
{
	struct MessageComponent;
}

namespace hidden
{
	struct GroupComponent;
	struct ObjectComponent;
	struct RevealedComponent;
}

namespace hidden
{
	class TrackerSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::SpriteComponent,
			gui::modal::MessageComponent,
			const eng::PrototypeComponent,
			const hidden::GroupComponent,
			const hidden::ObjectComponent,
			const hidden::RevealedComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}