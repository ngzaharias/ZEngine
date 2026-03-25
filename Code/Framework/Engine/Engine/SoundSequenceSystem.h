#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class AssetManager;
}

namespace eng::sound
{
	struct SequenceComponent;
	struct SequenceBufferComponent;
	struct SequenceRequestComponent;
}

namespace eng::sound
{
	class SequenceSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::AssetManager,
			eng::sound::SequenceBufferComponent,
			eng::sound::SequenceComponent>
			::Read<
			eng::sound::SequenceRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};