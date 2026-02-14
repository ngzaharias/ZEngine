#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::entity
{
	struct OpenWindowEvent;
}

namespace editor::flipbook
{
	struct OpenWindowEvent;
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

namespace editor
{
	class ToolbarSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::entity::OpenWindowEvent,
			editor::flipbook::OpenWindowEvent,
			editor::sprite::OpenWindowEvent,
			editor::texture::OpenWindowEvent,
			eng::settings::DebugSingleton>
			::Read<>;

		void Update(World& world, const GameTime& gameTime);
	};
}
