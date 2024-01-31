#pragma once

#include <ECS/System.h>

namespace eng
{
	struct CameraComponent;
	struct InputComponent;
	struct LinesComponent;
	struct TransformComponent;
}

namespace voxel
{
	struct ChunkComponent;
	struct ModifyComponent;

	class ModifySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::LinesComponent,
			voxel::ModifyComponent,
			const eng::CameraComponent,
			const eng::InputComponent,
			const eng::TransformComponent,
			const voxel::ChunkComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}