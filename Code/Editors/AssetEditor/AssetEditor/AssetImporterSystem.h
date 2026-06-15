#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor::assets
{
	struct ImporterComponent;
}

namespace eng
{
	class AssetManager;
	struct FileDropEvent;
}

namespace editor::assets
{
	class ImporterSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::assets::ImporterComponent,
			eng::AssetManager>
			::Read<
			eng::FileDropEvent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
