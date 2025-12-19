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
	struct TrajectoryAssetOpenComponent;
	struct TrajectoryAssetSaveComponent;
	struct TrajectoryWindowComponent;
	struct TrajectoryWindowRequest;
}

namespace eng
{
	class AssetManager;
}

namespace projectile
{
	struct CreateRequestComponent;
}

namespace editor
{
	class TrajectoryEditorSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// managers
			eng::AssetManager,
			// components
			ecs::NameComponent,
			editor::TrajectoryAssetOpenComponent,
			editor::TrajectoryAssetSaveComponent,
			editor::TrajectoryWindowComponent,
			projectile::CreateRequestComponent,
			const editor::TrajectoryWindowRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
};