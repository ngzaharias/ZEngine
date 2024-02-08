#pragma once

#include <ECS/System.h>

namespace eng
{
	struct CameraComponent;
	struct InputComponent;
	struct LinesComponent;
	struct TextComponent;
	struct TransformComponent;
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
			const eng::CameraComponent,
			const eng::InputComponent,
			const voxel::ChunkComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}