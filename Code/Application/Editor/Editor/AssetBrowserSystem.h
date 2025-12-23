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

namespace editor
{
	struct AssetBrowserWindowComponent;
	struct AssetBrowserWindowRequest;
}

namespace editor
{
	class AssetBrowserSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::AssetBrowserWindowComponent>
			::Read<
			eng::AssetManager,
			editor::AssetBrowserWindowRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
