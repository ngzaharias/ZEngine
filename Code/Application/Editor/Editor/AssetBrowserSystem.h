#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/System.h"
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
	struct AssetBrowserWindowRequestComponent;
}

namespace editor
{
	struct AssetBrowserWindowRequestComponent;

	struct AssetBrowserWindowComponent : public ecs::Component<AssetBrowserWindowComponent>
	{
		int32 m_Identifier = 0;
		str::String m_Label = {};

		Array<eng::AssetFile> m_Sorted = {};
	};

	class AssetBrowserSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// resources
			const eng::AssetManager,
			// components
			ecs::NameComponent,
			editor::AssetBrowserWindowComponent,
			const editor::AssetBrowserWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
