#include "EnginePCH.h"
#include "Engine/RegisterSystems.h"

#include <ECS/EntityWorld.h>
#include <ECS/WorldView.h>

#include "Engine/AssetManager.h"
#include "Engine/CameraSystem.h"
#include "Engine/FlipbookSystem.h"
#include "Engine/GLFW/Window.h"
#include "Engine/InputSystem.h"
#include "Engine/LevelLoadSystem.h"
#include "Engine/NetworkSystem.h"
#include "Engine/PhysicsSystem.h"
#include "Engine/RenderSystem.h"
#include "Engine/ReplicationHost.h"
#include "Engine/ReplicationPeer.h"
#include "Engine/Screen.h"
#include "Engine/SoundSystem.h"
#include "Engine/VoxelLoadingSystem.h"
#include "Engine/VoxelMeshingSystem.h"

void eng::RegisterClientSystems(ecs::EntityWorld& entityWorld, const eng::ClientDependencies& dependencies)
{
	entityWorld.RegisterSystem<eng::SoundSystem>();
	entityWorld.RegisterSystem<eng::CameraSystem>();
	entityWorld.RegisterSystem<eng::InputSystem>(
		dependencies.m_Window);
	entityWorld.RegisterSystem<network::NetworkSystem>();
	entityWorld.RegisterSystem<eng::FlipbookSystem>();
	entityWorld.RegisterSystem<eng::RenderSystem>(
		entityWorld,
		dependencies.m_Window);
	entityWorld.RegisterSystem<voxel::LoadingSystem>();
	entityWorld.RegisterSystem<voxel::MeshingSystem>();

	// render system needs to run after most systems
	entityWorld.RegisterSystemPriority<eng::RenderSystem>(5000);
}

void eng::RegisterServerSystems(ecs::EntityWorld& entityWorld, const eng::ServerDependencies& dependencies)
{
}

void eng::RegisterSharedSystems(ecs::EntityWorld& entityWorld, const eng::SharedDependencies& dependencies)
{
	entityWorld.RegisterSystem<eng::level::LoadSystem>(entityWorld);
	entityWorld.RegisterSystem<eng::PhysicsSystem>();
}