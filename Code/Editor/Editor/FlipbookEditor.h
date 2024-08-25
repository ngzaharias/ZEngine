#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"
#include "Engine/FlipbookAsset.h"

namespace eng
{
	class AssetManager;
}

namespace editor
{
	struct FlipbookWindowRequestComponent;

	struct FlipbookAssetOpenComponent : public ecs::Component<FlipbookAssetOpenComponent> { };
	struct FlipbookAssetSaveComponent : public ecs::Component<FlipbookAssetSaveComponent> { };
	struct FlipbookBatchingComponent : public ecs::Component<FlipbookBatchingComponent>
	{
		/// \brief How many iterations are done along X and Y.
		/// X is iterated again for each iteration of Y.
		Vector2u m_Iterations = Vector2u(1, 1);
		/// \brief Initial position of the batching pass.
		Vector2f m_Initial = Vector2f(100.f);
		/// \brief The stride between each frame.
		Vector2f m_Stride = Vector2f(100.f);

		/// \brief The size of each frame.
		Vector2f m_Size = Vector2f(100.f);
		/// \brief The number of frames.
		int32 m_Count = 1;

		/// \brief If it should be previewed.
		bool m_IsPreviewing = true;
	};

	struct FlipbookWindowComponent : public ecs::Component<FlipbookWindowComponent>
	{
		eng::FlipbookAsset m_Asset;

		str::String m_BatchingLabel = {};
		str::String m_DockspaceLabel = {};
		str::String m_InspectorLabel = {};
		str::String m_PreviewerLabel = {};
		str::String m_TextureLabel = {};
	};

	class FlipbookEditor final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// managers
			eng::AssetManager,
			// components
			editor::FlipbookAssetOpenComponent,
			editor::FlipbookAssetSaveComponent,
			editor::FlipbookBatchingComponent,
			editor::FlipbookWindowComponent,
			const editor::FlipbookWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
