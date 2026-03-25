#include "EnginePCH.h"
#include "Engine/RegisterComponents.h"

#include "Core/MemBuffer.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/ReplicationComponent.h"
#include "Engine/ApplicationCloseEvent.h"
#include "Engine/AssetComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/DynamicMeshComponent.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/FrameBufferComponent.h"
#include "Engine/LevelDirectoryComponent.h"
#include "Engine/LevelEntityComponent.h"
#include "Engine/LevelLoadedComponent.h"
#include "Engine/LevelLoadingComponent.h"
#include "Engine/LevelLoadEvent.h"
#include "Engine/LightAmbientComponent.h"
#include "Engine/LightDirectionalComponent.h"
#include "Engine/LightPointComponent.h"
#include "Engine/LinesComponent.h"
#include "Engine/MusicComponent.h"
#include "Engine/PhysicsComponent.h"
#include "Engine/PhysicsSceneComponent.h"
#include "Engine/PrototypeManager.h"
#include "Engine/RigidDynamicComponent.h"
#include "Engine/RigidStaticComponent.h"
#include "Engine/SavegameComponent.h"
#include "Engine/SettingsAudioComponent.h"
#include "Engine/SettingsDebugComponent.h"
#include "Engine/SettingsGameplayComponent.h"
#include "Engine/SettingsLaunchComponent.h"
#include "Engine/SettingsWindowComponent.h"
#include "Engine/SoundObjectComponent.h"
#include "Engine/SoundRandomBufferComponent.h"
#include "Engine/SoundRandomComponent.h"
#include "Engine/SoundRandomRequestComponent.h"
#include "Engine/SoundSequenceBufferComponent.h"
#include "Engine/SoundSequenceComponent.h"
#include "Engine/SoundSequenceRequestComponent.h"
#include "Engine/SoundSingleRequestComponent.h"
#include "Engine/SpriteComponent.h"
#include "Engine/StaticMeshComponent.h"
#include "Engine/TablesReloadedEvent.h"
#include "Engine/TextComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/UserComponent.h"
#include "Engine/UserMapComponent.h"
#include "Engine/VersionComponent.h"
#include "Engine/VisibilityComponent.h"

#include "Engine/Visitor.h"

template<>
void eng::Visitor::ReadCustom(ecs::ReplicationComponent& value) const
{
}

template<>
void eng::Visitor::WriteCustom(const ecs::ReplicationComponent& value)
{
}

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

	entityWorld.RegisterEvent<eng::application::CloseEvent>();
	entityWorld.RegisterEvent<eng::TablesReloadedEvent>();

	entityWorld.RegisterComponent<eng::FrameBufferComponent>();
	entityWorld.RegisterComponent<eng::LinesComponent>();
	entityWorld.RegisterComponent<eng::MusicComponent>();
	entityWorld.RegisterComponent<eng::settings::AudioComponent>();
	entityWorld.RegisterComponent<eng::settings::DebugComponent>();
	entityWorld.RegisterComponent<eng::settings::GameplayComponent>();
	entityWorld.RegisterComponent<eng::settings::WindowComponent>();
	entityWorld.RegisterComponent<eng::sound::RandomBufferComponent>();
	entityWorld.RegisterComponent<eng::sound::SequenceBufferComponent>();
}

void eng::RegisterServerComponents(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterComponent<net::UserMapComponent>();
}

void eng::RegisterSharedComponents(ecs::EntityWorld& entityWorld)
{
	// components
	{
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
	}

	// events
	{
		entityWorld.RegisterEvent<eng::level::LoadEvent>();
	}

	// components
	{
		entityWorld.RegisterComponent<eng::level::DirectoryComponent>();
		entityWorld.RegisterComponent<eng::PhysicsSceneComponent>();
		entityWorld.RegisterComponent<eng::settings::LaunchComponent>();
		entityWorld.RegisterComponent<eng::VersionComponent>();
	}
}
