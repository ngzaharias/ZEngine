#pragma once

#include "ECS/System.h"
#include "imgui/Identifier.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
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
		using World = ecs::WorldView<
			// Resources
			eng::AssetManager,
			const eng::InputManager,
			// Components
			ecs::NameComponent,
			editor::SpriteAssetNewComponent,
			editor::SpriteAssetOpenComponent,
			editor::SpriteAssetSaveComponent,
			editor::SpriteBatchingComponent,
			editor::SpriteWindowComponent,
			// Events
			const editor::SpriteWindowRequest,
			// Singletons
			editor::settings::LocalSingleton>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
