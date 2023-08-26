#pragma once

#include <ECS/Component.h>
#include <ECS/Entity.h>
#include <ECS/System.h>

namespace ecs
{
	class EntityWorld;
}

namespace eng
{
	struct LevelDirectoryComponent;
	struct LevelLoadRequestComponent;
}

namespace dbg
{
	struct LevelDialogRequestComponent;

	class OpenLevelSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::LevelLoadRequestComponent,
			const dbg::LevelDialogRequestComponent,
			const eng::LevelDirectoryComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}