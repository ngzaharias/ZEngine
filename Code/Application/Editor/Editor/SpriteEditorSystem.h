#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor
{
	struct SpriteAssetNewComponent;
	struct SpriteAssetOpenComponent;
	struct SpriteAssetSaveComponent;
	struct SpriteBatchingComponent;
	struct SpriteWindowComponent;
	struct SpriteWindowRequest;
}

namespace editor::settings
{
	struct LocalSingleton;
}

namespace eng
{
	class AssetManager;
	class InputManager;
}

namespace editor
{
	class SpriteEditorSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::settings::LocalSingleton,
			editor::SpriteAssetNewComponent,
			editor::SpriteAssetOpenComponent,
			editor::SpriteAssetSaveComponent,
			editor::SpriteBatchingComponent,
			editor::SpriteWindowComponent,
			eng::AssetManager>
			::Read<
			eng::InputManager,
			editor::SpriteWindowRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
