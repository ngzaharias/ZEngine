#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::importer
{
	struct WindowComponent;
}

namespace eng
{
	class AssetManager;
	struct FileDropEvent;
}

namespace editor::importer
{
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::importer::WindowComponent,
			eng::AssetManager>
			::Read<
			eng::FileDropEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
