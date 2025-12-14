#pragma once

#include "ECS/System.h"
#include "Engine/AssetManager.h"
#include "imgui/Identifier.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
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
		using World = ecs::WorldView<
			// resources
			const eng::AssetManager,
			// components
			ecs::NameComponent,
			editor::AssetBrowserWindowComponent,
			const editor::AssetBrowserWindowRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
