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

void shared::voxel::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<shared::voxel::ChunkChangedComponent>();
	world.RegisterComponent<shared::voxel::ChunkComponent>();
	world.RegisterComponent<shared::voxel::ChunkLoadedComponent>();
	world.RegisterComponent<shared::voxel::ChunkTemplate>();
	world.RegisterComponent<shared::voxel::ModifyComponent>();
	world.RegisterComponent<shared::voxel::ModifySettingsComponent>();
	world.RegisterSystem<shared::voxel::MeshingSystem>();
	world.RegisterSystem<shared::voxel::ModifySystem>();

	// templates
	{
		auto& manager = world.WriteResource<eng::TemplateManager>();
		manager.RegisterComponent<shared::voxel::ChunkTemplate>();
	}
}