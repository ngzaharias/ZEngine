#include "EnginePCH.h"
#include "Engine/RegisterSystems.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/AssetSystem.h"
#include "Engine/CameraSystem.h"
#include "Engine/FlipbookSystem.h"
#include "Engine/LevelLoadSystem.h"
#include "Engine/LightSystem.h"
#include "Engine/MusicSystem.h"
#include "Engine/PhysicsSystem.h"
#include "Engine/SettingsLaunchSystem.h"
#include "Engine/SoundPlaySystem.h"
#include "Engine/SoundPlaySystem.h"
#include "Engine/SoundRandomSystem.h"
#include "Engine/SoundSequenceSystem.h"
#include "Engine/SpriteSystem.h"
#include "Engine/StaticMeshSystem.h"
#include "Engine/TemplateSystem.h"
#include "Engine/TransformSystem.h"
#include "Engine/VersionSystem.h"
#include "Engine/VisibilitySystem.h"
#include "Engine/WindowSystem.h"

void eng::RegisterClientSystems(ecs::EntityWorld& world)
{
	world.RegisterSystem<eng::AssetSystem>();
	world.RegisterSystem<eng::CameraSystem>();
	world.RegisterSystem<eng::FlipbookSystem>();
	world.RegisterSystem<eng::LightSystem>();
	world.RegisterSystem<eng::MusicSystem>();
	world.RegisterSystem<eng::sound::PlaySystem>();
	world.RegisterSystem<eng::sound::RandomSystem>();
	world.RegisterSystem<eng::sound::SequenceSystem>();
	world.RegisterSystem<eng::SpriteSystem>();
	world.RegisterSystem<eng::StaticMeshSystem>();
	world.RegisterSystem<eng::VersionSystem>();
	world.RegisterSystem<eng::VisibilitySystem>();
	world.RegisterSystem<eng::WindowSystem>();
}

void eng::RegisterServerSystems(ecs::EntityWorld& world)
{
}

void eng::RegisterSharedSystems(ecs::EntityWorld& world)
{
	world.RegisterSystem<eng::level::LoadSystem>(world);
	world.RegisterSystem<eng::PhysicsSystem>();
	world.RegisterSystem<eng::settings::LaunchSystem>();
	world.RegisterSystem<eng::TemplateSystem>();
	world.RegisterSystem<eng::TransformSystem>();
}