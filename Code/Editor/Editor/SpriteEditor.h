#pragma once

#include <Core/Array.h>
#include <Core/Nullable.h>

#include <ECS/Component.h>
#include <ECS/System.h>

#include <Engine/SpriteAsset.h>

namespace eng
{
	class AssetManager;
	struct InputComponent;
}

namespace editor
{
	struct SpriteWindowRequestComponent;

	struct SpriteAssetOpenComponent : public ecs::Component<SpriteAssetOpenComponent> { };
	struct SpriteAssetSaveComponent : public ecs::Component<SpriteAssetSaveComponent> { };

	struct SpriteWindowComponent : public ecs::Component<SpriteWindowComponent>
	{
		eng::SpriteAsset m_Asset;

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
			editor::SpriteAssetOpenComponent,
			editor::SpriteAssetSaveComponent,
			editor::SpriteWindowComponent,
			const editor::SpriteWindowRequestComponent,
			const eng::InputComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
