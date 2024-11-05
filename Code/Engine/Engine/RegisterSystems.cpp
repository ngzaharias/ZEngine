#include "EnginePCH.h"
#include "Engine/RegisterSystems.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/AssetLoadSystem.h"
#include "Engine/CameraBound2DSystem.h"
#include "Engine/CameraMove2DSystem.h"
#include "Engine/CameraMove3DSystem.h"
#include "Engine/CameraPanningSystem.h"
#include "Engine/CameraZoom2DSystem.h"
#include "Engine/FlipbookSystem.h"
#include "Engine/InputSystem.h"
#include "Engine/LevelLoadSystem.h"
#include "Engine/MusicSystem.h"
#include "Engine/NetworkSystem.h"
#include "Engine/PhysicsSystem.h"
#include "Engine/RenderSystem.h"
#include "Engine/SettingsDebugSystem.h"
#include "Engine/SettingsLocalSystem.h"
#include "Engine/SoundPlaySystem.h"
#include "Engine/SoundRandomSystem.h"
#include "Engine/SoundSequenceSystem.h"
#include "Engine/VersionSystem.h"
#include "Engine/VoxelLoadingSystem.h"
#include "Engine/VoxelMeshingSystem.h"

void eng::RegisterClientSystems(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterSystem<eng::AssetLoadSystem>();
	entityWorld.RegisterSystem<eng::camera::Bound2DSystem>();
	entityWorld.RegisterSystem<eng::camera::Move2DSystem>();
	entityWorld.RegisterSystem<eng::camera::Move3DSystem>();
	entityWorld.RegisterSystem<eng::camera::PanningSystem>();
	entityWorld.RegisterSystem<eng::camera::Zoom2DSystem>();
	entityWorld.RegisterSystem<eng::InputSystem>();
	entityWorld.RegisterSystem<eng::FlipbookSystem>();
	entityWorld.RegisterSystem<eng::MusicSystem>();
	entityWorld.RegisterSystem<eng::network::NetworkSystem>();
	entityWorld.RegisterSystem<eng::RenderSystem>(entityWorld);
	entityWorld.RegisterSystem<eng::settings::DebugSystem>();
	entityWorld.RegisterSystem<eng::settings::LocalSystem>();
	entityWorld.RegisterSystem<eng::sound::PlaySystem>();
	entityWorld.RegisterSystem<eng::sound::RandomSystem>();
	entityWorld.RegisterSystem<eng::sound::SequenceSystem>();
	entityWorld.RegisterSystem<eng::VersionSystem>();
	entityWorld.RegisterSystem<voxel::LoadingSystem>();
	entityWorld.RegisterSystem<voxel::MeshingSystem>();

	// RandomSystem and SequenceSystem systems need to run before PlaySystem
	entityWorld.RegisterSystemPriority<eng::sound::RandomSystem>(4000);
	entityWorld.RegisterSystemPriority<eng::sound::SequenceSystem>(4000);
	entityWorld.RegisterSystemPriority<eng::sound::PlaySystem>(4001);

	// BehaviourSystem and PanningSystem need to run before BoundsSystem
	entityWorld.RegisterSystemPriority<eng::camera::Move2DSystem>(4000);
	entityWorld.RegisterSystemPriority<eng::camera::Move3DSystem>(4000);
	entityWorld.RegisterSystemPriority<eng::camera::PanningSystem>(4000);
	entityWorld.RegisterSystemPriority<eng::camera::Bound2DSystem>(4001);

	// render system needs to run after most systems
	entityWorld.RegisterSystemPriority<eng::RenderSystem>(5000);
}

void eng::RegisterServerSystems(ecs::EntityWorld& entityWorld)
{
}

void eng::RegisterSharedSystems(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterSystem<eng::level::LoadSystem>(entityWorld);
	entityWorld.RegisterSystem<eng::PhysicsSystem>();
}