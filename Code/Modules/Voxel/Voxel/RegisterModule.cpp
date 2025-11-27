#include "VoxelPCH.h"
#include "Voxel/RegisterModule.h"

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

void voxel::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// systems
	{
		entityWorld.RegisterSystem<voxel::MeshingSystem>();
		entityWorld.RegisterSystem<voxel::ModifySystem>();
	}

	// components
	{
		entityWorld.RegisterComponent<voxel::ChunkChangedFrameComponent>();
		entityWorld.RegisterComponent<voxel::ChunkComponent>();
		entityWorld.RegisterComponent<voxel::ChunkLoadedFrameComponent>();
		entityWorld.RegisterComponent<voxel::ModifyComponent>();
	}

	// singletons
	{
		entityWorld.RegisterSingleton<voxel::ModifySettingsSingleton>();
	}

	// prototype
	{
		auto& prototypeManager = entityWorld.WriteResource<eng::PrototypeManager>();
		prototypeManager.Register<voxel::ChunkComponent>();
	}
}