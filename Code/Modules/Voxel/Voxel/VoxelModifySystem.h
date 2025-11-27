#pragma once

#include "ECS/System.h"

namespace eng
{
	class InputManager;
	class WindowManager;
	struct LinesSingleton;
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
	struct ModifySettingsSingleton;
}

namespace voxel
{
	class ModifySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::InputManager,
			const eng::WindowManager,
			// Components
			eng::TextComponent,
			eng::TransformComponent,
			voxel::ModifyComponent,
			const eng::camera::ProjectionComponent,
			const voxel::ChunkComponent,
			// Singletons
			eng::LinesSingleton,
			voxel::ModifySettingsSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}