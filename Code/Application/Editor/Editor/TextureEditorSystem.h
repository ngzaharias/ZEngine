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
	struct TextureAssetImportComponent;
	struct TextureAssetNewComponent;
	struct TextureAssetOpenComponent;
	struct TextureAssetSaveComponent;
	struct TextureWindowComponent;
	struct TextureWindowRequest;
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
	class TextureEditorSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::AssetManager,
			const eng::InputManager,
			// Components
			ecs::NameComponent,
			editor::TextureAssetImportComponent,
			editor::TextureAssetNewComponent,
			editor::TextureAssetOpenComponent,
			editor::TextureAssetSaveComponent,
			editor::TextureWindowComponent,
			// Events
			const editor::TextureWindowRequest,
			// Singletons
			editor::settings::LocalSingleton>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
