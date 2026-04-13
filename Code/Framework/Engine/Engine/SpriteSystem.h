#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct SpriteComponent;
	struct SpriteTemplate;
}

namespace eng
{
	/// \brief
	class SpriteSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::SpriteComponent>
			::Read<
			eng::SpriteTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
}