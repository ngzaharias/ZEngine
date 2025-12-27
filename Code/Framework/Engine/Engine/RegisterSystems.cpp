#include "EnginePCH.h"
#include "Engine/RegisterSystems.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/AssetSystem.h"
#include "Engine/CameraBound2DSystem.h"
#include "Engine/CameraMove2DSystem.h"
#include "Engine/CameraMove3DSystem.h"
#include "Engine/CameraPanningSystem.h"
#include "Engine/CameraZoom2DSystem.h"
#include "Engine/FlipbookPlaySystem.h"
#include "Engine/LevelLoadSystem.h"
#include "Engine/MusicSystem.h"
#include "Engine/NetworkSystem.h"
#include "Engine/PhysicsSystem.h"
#include "Engine/ReplicationHost.h"
#include "Engine/ReplicationPeer.h"
#include "Engine/SoundPlaySystem.h"
#include "Engine/SettingsLaunchSystem.h"
#include "Engine/SoundPlaySystem.h"
#include "Engine/SoundRandomSystem.h"
#include "Engine/SoundSequenceSystem.h"
#include "Engine/VersionSystem.h"
#include "Engine/WindowSystem.h"

void eng::RegisterClientSystems(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterSystem<eng::AssetSystem>();
	entityWorld.RegisterSystem<eng::camera::Bound2DSystem>();
	entityWorld.RegisterSystem<eng::camera::Move2DSystem>();
	entityWorld.RegisterSystem<eng::camera::Move3DSystem>();
	entityWorld.RegisterSystem<eng::camera::PanningSystem>();
	entityWorld.RegisterSystem<eng::camera::Zoom2DSystem>();
	entityWorld.RegisterSystem<eng::FlipbookPlaySystem>();
	entityWorld.RegisterSystem<eng::MusicSystem>();
	entityWorld.RegisterSystem<eng::network::NetworkSystem>();
	entityWorld.RegisterSystem<eng::settings::LaunchSystem>();
	entityWorld.RegisterSystem<eng::sound::PlaySystem>();
	entityWorld.RegisterSystem<eng::sound::RandomSystem>();
	entityWorld.RegisterSystem<eng::sound::SequenceSystem>();
	entityWorld.RegisterSystem<eng::VersionSystem>();
	entityWorld.RegisterSystem<eng::WindowSystem>();

	// Move2DSystem, Move3DSystem and PanningSystem need to run before BoundsSystem
	//entityWorld.RegisterSystemPriority<eng::camera::Move2DSystem>(4000);
	//entityWorld.RegisterSystemPriority<eng::camera::Move3DSystem>(4000);
	//entityWorld.RegisterSystemPriority<eng::camera::PanningSystem>(4000);
	//entityWorld.RegisterSystemPriority<eng::camera::Bound2DSystem>(4001);
}

void eng::RegisterServerSystems(ecs::EntityWorld& entityWorld)
{
}

void eng::RegisterSharedSystems(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterSystem<eng::level::LoadSystem>(entityWorld);
	entityWorld.RegisterSystem<eng::PhysicsSystem>();
}