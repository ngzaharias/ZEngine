#include "VoxelPCH.h"
#include "Voxel/VoxelRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Voxel/VoxelChunkChangedFrameComponent.h"
#include "Voxel/VoxelChunkComponent.h"
#include "Voxel/VoxelChunkLoadedFrameComponent.h"
#include "Voxel/VoxelModifyComponent.h"
#include "Voxel/VoxelModifySettingsSingleton.h"
#include "Voxel/VoxelMeshingSystem.h"
#include "Voxel/VoxelModifySystem.h"

void voxel::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<voxel::ChunkChangedFrameComponent>();
		world.RegisterComponent<voxel::ChunkComponent>();
		world.RegisterComponent<voxel::ChunkLoadedFrameComponent>();
		world.RegisterComponent<voxel::ModifyComponent>();
	}

	// singletons
	{
		world.RegisterSingleton<voxel::ModifySettingsSingleton>();
	}

	// systems
	{
		world.RegisterSystem<voxel::MeshingSystem>();
		world.RegisterSystem<voxel::ModifySystem>();
	}

	// prototype
	{
		auto& manager = world.WriteResource<eng::PrototypeManager>();
		manager.Register<voxel::ChunkComponent>();
	}
}