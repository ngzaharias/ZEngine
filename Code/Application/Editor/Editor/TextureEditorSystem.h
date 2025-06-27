#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "ECS/System.h"
#include "Engine/Texture2DAsset.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor::settings
{
	struct LocalComponent;
}

namespace eng
{
	class AssetManager;
	class InputManager;
}

namespace editor
{
	struct TextureWindowRequestComponent;

	struct TextureAssetImportComponent : public ecs::Component<TextureAssetImportComponent> { };
	struct TextureAssetNewComponent : public ecs::Component<TextureAssetNewComponent> { };
	struct TextureAssetOpenComponent : public ecs::Component<TextureAssetOpenComponent> { };
	struct TextureAssetSaveComponent : public ecs::Component<TextureAssetSaveComponent> { };

	struct TextureWindowComponent : public ecs::Component<TextureWindowComponent>
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
			// resources
			eng::AssetManager,
			const eng::InputManager,
			// components
			ecs::NameComponent,
			editor::settings::LocalComponent,
			editor::TextureAssetImportComponent,
			editor::TextureAssetNewComponent,
			editor::TextureAssetOpenComponent,
			editor::TextureAssetSaveComponent,
			editor::TextureWindowComponent,
			const editor::TextureWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
