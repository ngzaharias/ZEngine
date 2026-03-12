#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::assets
{
	struct OpenWindowEvent;
}

namespace editor::flipbook
{
	struct OpenWindowEvent;
}

namespace editor::inspector
{
	struct OpenWindowEvent;
}

namespace editor::outliner
{
	struct OpenWindowEvent;
}

namespace editor::settings
{
	struct LocalSingleton;
}

namespace editor::sprite
{
	struct OpenWindowEvent;
}

namespace editor::texture
{
	struct OpenWindowEvent;
}

namespace eng::settings
{
	struct DebugSingleton;
}

namespace gamestate
{
	struct EditModeComponent;
	struct EditModeToggleEvent;
}

namespace editor
{
	class ToolbarSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::assets::OpenWindowEvent,
			editor::flipbook::OpenWindowEvent,
			editor::inspector::OpenWindowEvent,
			editor::outliner::OpenWindowEvent,
			editor::settings::LocalSingleton,
			editor::sprite::OpenWindowEvent,
			editor::texture::OpenWindowEvent,
			eng::settings::DebugSingleton,
			gamestate::EditModeComponent,
			gamestate::EditModeToggleEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
