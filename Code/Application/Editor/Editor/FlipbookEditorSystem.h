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
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::FlipbookAssetOpenComponent,
			editor::FlipbookAssetSaveComponent,
			editor::FlipbookBatchingComponent,
			editor::FlipbookWindowComponent,
			eng::AssetManager>
			::Read<
			editor::FlipbookWindowRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
