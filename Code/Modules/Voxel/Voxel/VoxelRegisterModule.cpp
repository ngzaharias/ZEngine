#include "VoxelPCH.h"
#include "Voxel/VoxelRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TemplateManager.h"
#include "Voxel/VoxelChunkChangedComponent.h"
#include "Voxel/VoxelChunkComponent.h"
#include "Voxel/VoxelChunkLoadedComponent.h"
#include "Voxel/VoxelChunkTemplate.h"
#include "Voxel/VoxelMeshingSystem.h"
#include "Voxel/VoxelModifyComponent.h"
#include "Voxel/VoxelModifySettingsComponent.h"
#include "Voxel/VoxelModifySystem.h"

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