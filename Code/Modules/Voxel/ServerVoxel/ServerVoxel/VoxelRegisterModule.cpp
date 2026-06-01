#include "VoxelPCH.h"
#include "ServerVoxel/VoxelRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "ServerVoxel/VoxelChunkSystem.h"

void server::voxel::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterSystem<server::voxel::ChunkSystem>();
}