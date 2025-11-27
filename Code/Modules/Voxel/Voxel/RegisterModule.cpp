#include "VoxelPCH.h"
#include "Voxel/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Voxel/VoxelComponents.h"
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
		entityWorld.RegisterComponent<voxel::ChunkChangedEventComponent>();
		entityWorld.RegisterComponent<voxel::ChunkComponent>();
		entityWorld.RegisterComponent<voxel::ChunkLoadedEventComponent>();
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