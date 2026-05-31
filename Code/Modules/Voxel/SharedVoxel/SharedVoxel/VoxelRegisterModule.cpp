#include "VoxelPCH.h"
#include "SharedVoxel/VoxelRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TemplateManager.h"
#include "SharedVoxel/VoxelChunkChangedComponent.h"
#include "SharedVoxel/VoxelChunkComponent.h"
#include "SharedVoxel/VoxelChunkLoadedComponent.h"
#include "SharedVoxel/VoxelChunkTemplate.h"
#include "SharedVoxel/VoxelMeshingSystem.h"
#include "SharedVoxel/VoxelModifyComponent.h"
#include "SharedVoxel/VoxelModifySettingsComponent.h"
#include "SharedVoxel/VoxelModifySystem.h"

void voxel::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<voxel::ChunkChangedComponent>();
	world.RegisterComponent<voxel::ChunkComponent>();
	world.RegisterComponent<voxel::ChunkLoadedComponent>();
	world.RegisterComponent<voxel::ChunkTemplate>();
	world.RegisterComponent<voxel::ModifyComponent>();
	world.RegisterComponent<voxel::ModifySettingsComponent>();
	world.RegisterSystem<voxel::MeshingSystem>();
	world.RegisterSystem<voxel::ModifySystem>();

	// templates
	{
		auto& manager = world.WriteResource<eng::TemplateManager>();
		manager.RegisterComponent<voxel::ChunkTemplate>();
	}
}