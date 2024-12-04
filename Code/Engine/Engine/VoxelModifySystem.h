#pragma once

#include "ECS/System.h"

namespace eng
{
	struct InputComponent;
	struct LinesComponent;
	struct TextComponent;
	struct TransformComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace voxel
{
	struct ChunkComponent;
	struct ModifyComponent;
	struct ModifySettingsComponent;

	class ModifySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::LinesComponent,
			eng::TextComponent,
			eng::TransformComponent,
			voxel::ModifyComponent,
			voxel::ModifySettingsComponent,
			const eng::camera::ProjectionComponent,
			const eng::InputComponent,
			const voxel::ChunkComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}