#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::importer
{
	struct AdvanceEvent;
	struct QueueComponent;
}

namespace eng
{
	class AssetManager;
	struct FileDropEvent;
}

namespace editor::importer
{
	class QueueSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::importer::QueueComponent,
			eng::AssetManager>
			::Read<
			editor::importer::AdvanceEvent,
			eng::FileDropEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
