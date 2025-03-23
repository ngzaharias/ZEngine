#pragma once

#include "ECS/System.h"

namespace eng
{
	class InputManager;
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
			// Resources
			const eng::InputManager,
			// Components
			eng::LinesComponent,
			eng::TextComponent,
			eng::TransformComponent,
			voxel::ModifyComponent,
			voxel::ModifySettingsComponent,
			const eng::camera::ProjectionComponent,
			const voxel::ChunkComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}