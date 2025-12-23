#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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
		using World = ecs::WorldView
			::Write<
			eng::LinesSingleton,
			eng::TextComponent,
			eng::TransformComponent,
			voxel::ModifyComponent,
			voxel::ModifySettingsSingleton>
			::Read<
			eng::camera::ProjectionComponent,
			eng::InputManager,
			eng::WindowManager,
			voxel::ChunkComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}