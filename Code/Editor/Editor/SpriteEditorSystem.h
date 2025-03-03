#pragma once

#include "Core/Array.h"
#include "Core/String.h"
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
	struct SpriteBatchingComponent : public ecs::Component<SpriteBatchingComponent>
	{
		/// \brief How many iterations are done along X and Y.
		/// X is iterated again for each iteration of Y.
		Vector2u m_Iterations = Vector2u::One;
		/// \brief Initial position of the batching pass.
		Vector2f m_Initial = Vector2f::Zero;
		/// \brief The stride between each sprite.
		Vector2f m_Stride = Vector2f(128.f);

		/// \brief The format for sprite names.
		str::String m_Format = "SP_Sprite_";
		/// \brief The number of sprites.
		int32 m_Count = 1;

		/// \brief If it should be previewed.
		bool m_IsPreviewing = true;
	};

	struct SpriteWindowComponent : public ecs::Component<SpriteWindowComponent>
	{
		int32 m_Identifier = 0;

		str::String m_DockspaceLabel = {};
		str::String m_InspectorLabel = {};
		str::String m_PreviewerLabel = {};
		str::String m_TextureLabel = {};

		eng::SpriteAsset m_Asset = {};
		str::Guid m_ShaderLoaded = {};
		str::Guid m_TextureLoaded = {};
	};

	class SpriteEditorSystem final : public ecs::System
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
			editor::SpriteBatchingComponent,
			editor::SpriteWindowComponent,
			const editor::SpriteWindowRequestComponent,
			const eng::InputComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
