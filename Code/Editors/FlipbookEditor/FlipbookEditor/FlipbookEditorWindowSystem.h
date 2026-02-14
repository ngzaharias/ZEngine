#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor::flipbook
{
	struct AssetOpenComponent;
	struct AssetSaveComponent;
	struct BatchingComponent;
	struct OpenWindowEvent;
	struct WindowComponent;
}

namespace eng
{
	class AssetManager;
}

namespace editor::flipbook
{
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::flipbook::AssetOpenComponent,
			editor::flipbook::AssetSaveComponent,
			editor::flipbook::BatchingComponent,
			editor::flipbook::WindowComponent,
			eng::AssetManager>
			::Read<
			editor::flipbook::OpenWindowEvent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
