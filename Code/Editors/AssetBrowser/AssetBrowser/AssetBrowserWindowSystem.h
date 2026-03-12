#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class AssetManager;
	struct AssetFile;
}

namespace editor::assets
{
	struct OpenWindowEvent;
	struct WindowComponent;
}

namespace editor::assets
{
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::assets::WindowComponent>
			::Read<
			editor::assets::OpenWindowEvent,
			eng::AssetManager>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
