#include "VoxelPCH.h"
#include "ClientVoxel/VoxelRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "ClientVoxel/VoxelMeshingSystem.h"
#include "ClientVoxel/VoxelModifySettingsComponent.h"
#include "ClientVoxel/VoxelModifySystem.h"

void client::voxel::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<client::voxel::ModifySettingsComponent>();
	world.RegisterSystem<client::voxel::MeshingSystem>();
	world.RegisterSystem<client::voxel::ModifySystem>();
}