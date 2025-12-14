#pragma once

#include "ECS/System.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor
{
	struct FlipbookAssetOpenComponent;
	struct FlipbookAssetSaveComponent;
	struct FlipbookBatchingComponent;
	struct FlipbookWindowComponent;
	struct FlipbookWindowRequest;
}

namespace eng
{
	class AssetManager;
}

namespace editor
{
	class FlipbookEditorSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// managers
			eng::AssetManager,
			// components
			ecs::NameComponent,
			editor::FlipbookAssetOpenComponent,
			editor::FlipbookAssetSaveComponent,
			editor::FlipbookBatchingComponent,
			editor::FlipbookWindowComponent,
			const editor::FlipbookWindowRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
