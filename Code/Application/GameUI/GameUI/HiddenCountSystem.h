#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	class UIManager;
}

namespace hidden
{
	struct CountComponent;
}

namespace gui::hidden
{
	class CountSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::UIManager,
			// Components
			const ::hidden::CountComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}