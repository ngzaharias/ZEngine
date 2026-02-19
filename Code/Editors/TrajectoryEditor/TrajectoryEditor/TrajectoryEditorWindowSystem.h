#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor::trajectory
{
	struct AssetOpenComponent;
	struct AssetSaveComponent;
	struct WindowComponent;
	struct WindowRequest;
}

namespace eng
{
	class AssetManager;
}

namespace gamestate
{
	struct EditorComponent;
}

namespace editor::trajectory
{
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::trajectory::AssetOpenComponent,
			editor::trajectory::AssetSaveComponent,
			editor::trajectory::WindowComponent,
			eng::AssetManager>
			::Read<
			editor::trajectory::WindowRequest,
			gamestate::EditorComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
};