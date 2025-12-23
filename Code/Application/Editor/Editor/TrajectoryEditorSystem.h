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
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::TrajectoryAssetOpenComponent,
			editor::TrajectoryAssetSaveComponent,
			editor::TrajectoryWindowComponent,
			eng::AssetManager,
			projectile::CreateRequestComponent>
			::Read<
			editor::TrajectoryWindowRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
};