#include "EnginePCH.h"
#include "Engine/RegisterSystems.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/AssetSystem.h"
#include "Engine/FlipbookPlaySystem.h"
#include "Engine/LevelLoadSystem.h"
#include "Engine/MusicSystem.h"
#include "Engine/PhysicsSystem.h"
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
	entityWorld.RegisterSystem<eng::FlipbookPlaySystem>();
	entityWorld.RegisterSystem<eng::MusicSystem>();
	entityWorld.RegisterSystem<eng::sound::PlaySystem>();
	entityWorld.RegisterSystem<eng::sound::RandomSystem>();
	entityWorld.RegisterSystem<eng::sound::SequenceSystem>();
	entityWorld.RegisterSystem<eng::VersionSystem>();
	entityWorld.RegisterSystem<eng::WindowSystem>();
}

void eng::RegisterServerSystems(ecs::EntityWorld& entityWorld)
{
}

void eng::RegisterSharedSystems(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterSystem<eng::level::LoadSystem>(entityWorld);
	entityWorld.RegisterSystem<eng::PhysicsSystem>();
	entityWorld.RegisterSystem<eng::settings::LaunchSystem>();
}