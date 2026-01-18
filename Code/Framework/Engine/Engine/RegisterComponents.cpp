#include "EnginePCH.h"
#include "Engine/RegisterComponents.h"

#include "Core/MemBuffer.h"
#include "ECS/DebugEvent.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/ReplicationComponent.h"
#include "Engine/ApplicationCloseRequest.h"
#include "Engine/AssetComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/DynamicMeshComponent.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/FrameBufferSingleton.h"
#include "Engine/LevelDirectorySingleton.h"
#include "Engine/LevelEntityComponent.h"
#include "Engine/LevelLoadedComponent.h"
#include "Engine/LevelLoadingComponent.h"
#include "Engine/LevelLoadRequest.h"
#include "Engine/LightAmbientComponent.h"
#include "Engine/LightDirectionalComponent.h"
#include "Engine/LightPointComponent.h"
#include "Engine/LinesComponent.h"
#include "Engine/MusicSingleton.h"
#include "Engine/NetworkChangeFinished.h"
#include "Engine/NetworkChangeRequest.h"
#include "Engine/NetworkStateSingleton.h"
#include "Engine/PhysicsComponent.h"
#include "Engine/PhysicsSceneComponent.h"
#include "Engine/PrototypeManager.h"
#include "Engine/RigidDynamicComponent.h"
#include "Engine/RigidStaticComponent.h"
#include "Engine/SavegameComponent.h"
#include "Engine/SettingsAudioSingleton.h"
#include "Engine/SettingsDebugSingleton.h"
#include "Engine/SettingsGameplaySingleton.h"
#include "Engine/SettingsLaunchSingleton.h"
#include "Engine/SettingsWindowSingleton.h"
#include "Engine/SoundObjectComponent.h"
#include "Engine/SoundRandomBufferSingleton.h"
#include "Engine/SoundRandomComponent.h"
#include "Engine/SoundRandomRequestComponent.h"
#include "Engine/SoundSequenceBufferSingleton.h"
#include "Engine/SoundSequenceComponent.h"
#include "Engine/SoundSequenceRequestComponent.h"
#include "Engine/SoundSingleRequestComponent.h"
#include "Engine/SpriteComponent.h"
#include "Engine/StaticMeshComponent.h"
#include "Engine/TablesReloaded.h"
#include "Engine/TextComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/UserComponent.h"
#include "Engine/UserMapSingleton.h"
#include "Engine/VersionComponent.h"
#include "Engine/VisibilityComponent.h"

void eng::RegisterClientComponents(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterComponent<eng::DynamicMeshComponent>();
	entityWorld.RegisterComponent<eng::light::AmbientComponent>();
	entityWorld.RegisterComponent<eng::light::DirectionalComponent>();
	entityWorld.RegisterComponent<eng::light::PointComponent>();
	entityWorld.RegisterComponent<eng::SavegameComponent>();
	entityWorld.RegisterComponent<eng::sound::ObjectComponent>();
	entityWorld.RegisterComponent<eng::sound::RandomComponent>();
	entityWorld.RegisterComponent<eng::sound::RandomRequestComponent>();
	entityWorld.RegisterComponent<eng::sound::SequenceComponent>();
	entityWorld.RegisterComponent<eng::sound::SequenceRequestComponent>();
	entityWorld.RegisterComponent<eng::sound::SingleRequestComponent>();
	entityWorld.RegisterComponent<eng::VisibilityComponent>();

	entityWorld.RegisterEvent<eng::application::CloseRequest>();
	entityWorld.RegisterEvent<eng::network::ChangeFinished>();
	entityWorld.RegisterEvent<eng::network::ChangeRequest>();
	entityWorld.RegisterEvent<eng::TablesReloaded>();

	entityWorld.RegisterSingleton<eng::FrameBufferSingleton>();
	entityWorld.RegisterSingleton<eng::LinesSingleton>();
	entityWorld.RegisterSingleton<eng::MusicSingleton>();
	entityWorld.RegisterSingleton<eng::network::StateSingleton>();
	entityWorld.RegisterSingleton<eng::settings::AudioSingleton>();
	entityWorld.RegisterSingleton<eng::settings::DebugSingleton>();
	entityWorld.RegisterSingleton<eng::settings::GameplaySingleton>();
	entityWorld.RegisterSingleton<eng::settings::LaunchSingleton>();
	entityWorld.RegisterSingleton<eng::settings::WindowSingleton>();
	entityWorld.RegisterSingleton<eng::sound::RandomBufferSingleton>();
	entityWorld.RegisterSingleton<eng::sound::SequenceBufferSingleton>();
}

void eng::RegisterServerComponents(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterSingleton<net::UserMapSingleton>();
}

void eng::RegisterSharedComponents(ecs::EntityWorld& entityWorld)
{
	// components
	entityWorld.RegisterComponent<ecs::ReplicationComponent>();
	entityWorld.RegisterComponent<eng::ActiveComponent>();
	entityWorld.RegisterComponent<eng::AssetComponent>();
	entityWorld.RegisterComponent<eng::CameraComponent>();
	entityWorld.RegisterComponent<eng::EditorComponent>();
	entityWorld.RegisterComponent<eng::FlipbookComponent>();
	entityWorld.RegisterComponent<eng::level::EntityComponent>();
	entityWorld.RegisterComponent<eng::level::LoadedComponent>();
	entityWorld.RegisterComponent<eng::level::LoadingComponent>();
	entityWorld.RegisterComponent<eng::PhysicsComponent>();
	entityWorld.RegisterComponent<eng::PrototypeComponent>();
	entityWorld.RegisterComponent<eng::RigidDynamicComponent>();
	entityWorld.RegisterComponent<eng::RigidStaticComponent>();
	entityWorld.RegisterComponent<eng::SpriteComponent>();
	entityWorld.RegisterComponent<eng::StaticMeshComponent>();
	entityWorld.RegisterComponent<eng::TextComponent>();
	entityWorld.RegisterComponent<eng::TransformComponent>();
	entityWorld.RegisterComponent<net::UserComponent>();

	entityWorld.RegisterEvent<ecs::DebugEvent>();
	entityWorld.RegisterEvent<eng::level::LoadRequest>();

	// singletons
	entityWorld.RegisterSingleton<eng::level::DirectorySingleton>();
	entityWorld.RegisterSingleton<eng::PhysicsSceneSingleton>();
	entityWorld.RegisterSingleton<eng::VersionSingleton>();
}
