#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "ECS/System.h"
#include "Engine/Texture2DAsset.h"
#include "imgui/Identifier.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
	struct NameComponent;
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
	struct TextureWindowRequest;

	struct TextureAssetImportComponent final : public ecs::Component<TextureAssetImportComponent> { };
	struct TextureAssetNewComponent final : public ecs::Component<TextureAssetNewComponent> { };
	struct TextureAssetOpenComponent final : public ecs::Component<TextureAssetOpenComponent> { };
	struct TextureAssetSaveComponent final : public ecs::Component<TextureAssetSaveComponent> { };

	struct TextureWindowComponent final : public ecs::Component<TextureWindowComponent>
	{
		int32 m_Identifier = 0;
		eng::Texture2DAsset m_Asset = {};

		str::String m_DockspaceLabel = {};
		str::String m_InspectorLabel = {};
		str::String m_PreviewerLabel = {};
	};

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
