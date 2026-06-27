#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::importer
{
	struct AdvanceEvent;
	struct QueueComponent;
	struct StaticMeshComponent;
}

namespace eng
{
	class AssetManager;
}

namespace editor::importer
{
	class StaticMeshSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::importer::AdvanceEvent,
			editor::importer::StaticMeshComponent,
			eng::AssetManager>
			::Read<
			editor::importer::QueueComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
