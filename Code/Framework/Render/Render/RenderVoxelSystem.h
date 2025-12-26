#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class AssetManager;
	class WindowManager;
	struct TransformComponent;
	struct DynamicMeshComponent;
}

namespace eng::camera
{
	struct EditorComponent;
	struct ProjectionComponent;
}

namespace eng::settings
{
	struct DebugSingleton;
}

namespace render
{
	struct OpaqueComponent;
	struct ShadowComponent;
}

namespace voxel
{
	struct ChunkComponent;
}

namespace render
{
	class VoxelSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::AssetManager,
			render::OpaqueComponent>
			::Read<
			eng::camera::EditorComponent,
			eng::camera::ProjectionComponent,
			eng::DynamicMeshComponent,
			eng::settings::DebugSingleton,
			eng::TransformComponent,
			eng::WindowManager,
			render::ShadowComponent,
			voxel::ChunkComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}