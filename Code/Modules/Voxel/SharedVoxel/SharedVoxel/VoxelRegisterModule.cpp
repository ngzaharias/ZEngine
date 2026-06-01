#include "VoxelPCH.h"
#include "SharedVoxel/VoxelRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TemplateManager.h"
#include "SharedVoxel/VoxelChunkComponent.h"
#include "SharedVoxel/VoxelChunkTemplate.h"
#include "SharedVoxel/VoxelClientModifyEvent.h"

void shared::voxel::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<shared::voxel::ChunkComponent>();
	world.RegisterComponent<shared::voxel::ChunkTemplate>();
	world.RegisterEvent<shared::voxel::ClientModifyEvent>();

	// templates
	{
		auto& manager = world.WriteResource<eng::TemplateManager>();
		manager.RegisterComponent<shared::voxel::ChunkTemplate>();
	}
}