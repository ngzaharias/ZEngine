#pragma once

#include "Core/Guid.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class AssetManager;
}

namespace eng::settings
{
	struct AudioStaticComponent;
}

namespace eng::sound
{
	struct ObjectComponent;
	struct RandomBufferStaticComponent;
	struct SequenceBufferStaticComponent;
	struct SingleRequestComponent;
}

namespace eng::sound
{
	class PlaySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::AssetManager,
			ecs::NameComponent,
			eng::sound::ObjectComponent>
			::Read<
			eng::settings::AudioStaticComponent,
			eng::sound::RandomBufferStaticComponent,
			eng::sound::SequenceBufferStaticComponent,
			eng::sound::SingleRequestComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		Array<str::Guid> m_Requests = {};
	};
};