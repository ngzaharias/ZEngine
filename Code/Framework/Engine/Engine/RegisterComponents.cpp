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
#include "Engine/FrameBufferStaticComponent.h"
#include "Engine/LevelDirectoryStaticComponent.h"
#include "Engine/LevelEntityComponent.h"
#include "Engine/LevelLoadedComponent.h"
#include "Engine/LevelLoadingComponent.h"
#include "Engine/LevelLoadEvent.h"
#include "Engine/LightAmbientComponent.h"
#include "Engine/LightDirectionalComponent.h"
#include "Engine/LightPointComponent.h"
#include "Engine/LinesStaticComponent.h"
#include "Engine/MusicStaticComponent.h"
#include "Engine/PhysicsComponent.h"
#include "Engine/PhysicsSceneStaticComponent.h"
#include "Engine/PrototypeManager.h"
#include "Engine/RigidDynamicComponent.h"
#include "Engine/RigidStaticComponent.h"
#include "Engine/SavegameComponent.h"
#include "Engine/SettingsAudioStaticComponent.h"
#include "Engine/SettingsDebugStaticComponent.h"
#include "Engine/SettingsGameplayStaticComponent.h"
#include "Engine/SettingsLaunchStaticComponent.h"
#include "Engine/SettingsWindowStaticComponent.h"
#include "Engine/SoundObjectComponent.h"
#include "Engine/SoundRandomBufferStaticComponent.h"
#include "Engine/SoundRandomComponent.h"
#include "Engine/SoundRandomRequestComponent.h"
#include "Engine/SoundSequenceBufferStaticComponent.h"
#include "Engine/SoundSequenceComponent.h"
#include "Engine/SoundSequenceRequestComponent.h"
#include "Engine/SoundSingleRequestComponent.h"
#include "Engine/SpriteComponent.h"
#include "Engine/StaticMeshComponent.h"
#include "Engine/TablesReloadedEvent.h"
#include "Engine/TextComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/UserComponent.h"
#include "Engine/UserMapStaticComponent.h"
#include "Engine/VersionStaticComponent.h"
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

	entityWorld.RegisterComponent<eng::FrameBufferStaticComponent>();
	entityWorld.RegisterComponent<eng::LinesStaticComponent>();
	entityWorld.RegisterComponent<eng::MusicStaticComponent>();
	entityWorld.RegisterComponent<eng::settings::AudioStaticComponent>();
	entityWorld.RegisterComponent<eng::settings::DebugStaticComponent>();
	entityWorld.RegisterComponent<eng::settings::GameplayStaticComponent>();
	entityWorld.RegisterComponent<eng::settings::WindowStaticComponent>();
	entityWorld.RegisterComponent<eng::sound::RandomBufferStaticComponent>();
	entityWorld.RegisterComponent<eng::sound::SequenceBufferStaticComponent>();
}

void eng::RegisterServerComponents(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterComponent<net::UserMapStaticComponent>();
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
		entityWorld.RegisterComponent<eng::level::DirectoryStaticComponent>();
		entityWorld.RegisterComponent<eng::PhysicsSceneStaticComponent>();
		entityWorld.RegisterComponent<eng::settings::LaunchStaticComponent>();
		entityWorld.RegisterComponent<eng::VersionStaticComponent>();
	}
}
