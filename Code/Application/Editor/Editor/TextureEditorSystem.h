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
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::settings::LocalSingleton,
			editor::TextureAssetImportComponent,
			editor::TextureAssetNewComponent,
			editor::TextureAssetOpenComponent,
			editor::TextureAssetSaveComponent,
			editor::TextureWindowComponent,
			eng::AssetManager>
			::Read<
			editor::TextureWindowRequest,
			eng::InputManager>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
