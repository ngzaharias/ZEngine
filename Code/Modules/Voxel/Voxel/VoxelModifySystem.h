#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct EditorComponent;
	struct LinesComponent;
	struct TextComponent;
	struct TransformComponent;
}

namespace voxel
{
	struct ChunkComponent;
	struct ModifyComponent;
	struct ModifySettingsComponent;
}

namespace voxel
{
	class ModifySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::InputManager,
			eng::LinesComponent,
			eng::TextComponent,
			eng::TransformComponent,
			voxel::ModifyComponent,
			voxel::ModifySettingsComponent>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::EditorComponent,
			eng::WindowManager,
			voxel::ChunkComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}