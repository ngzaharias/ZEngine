#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "ECS/System.h"
#include "Engine/SpriteAsset.h"
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
	struct InputComponent;
}

namespace editor
{
	struct SpriteWindowRequestComponent;

	struct SpriteAssetNewComponent : public ecs::Component<SpriteAssetNewComponent> { };
	struct SpriteAssetOpenComponent : public ecs::Component<SpriteAssetOpenComponent> { };
	struct SpriteAssetSaveComponent : public ecs::Component<SpriteAssetSaveComponent> { };

	struct SpriteWindowComponent : public ecs::Component<SpriteWindowComponent>
	{
		int32 m_Identifier = 0;
		eng::SpriteAsset m_Asset = {};

		str::String m_DockspaceLabel = {};
		str::String m_InspectorLabel = {};
		str::String m_PreviewerLabel = {};
		str::String m_TextureLabel = {};
	};

	class SpriteEditor final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// managers
			eng::AssetManager,
			// components
			ecs::NameComponent,
			editor::settings::LocalComponent,
			editor::SpriteAssetNewComponent,
			editor::SpriteAssetOpenComponent,
			editor::SpriteAssetSaveComponent,
			editor::SpriteWindowComponent,
			const editor::SpriteWindowRequestComponent,
			const eng::InputComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
