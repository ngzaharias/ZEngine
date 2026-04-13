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
#include "Engine/CameraTemplate.h"
#include "Engine/CameraTemplate.h"
#include "Engine/DynamicMeshComponent.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/FlipbookTemplate.h"
#include "Engine/FrameBufferComponent.h"
#include "Engine/LevelDirectoryComponent.h"
#include "Engine/LevelEntityComponent.h"
#include "Engine/LevelLoadedComponent.h"
#include "Engine/LevelLoadEvent.h"
#include "Engine/LevelLoadingComponent.h"
#include "Engine/LightAmbientComponent.h"
#include "Engine/LightAmbientTemplate.h"
#include "Engine/LightDirectionalComponent.h"
#include "Engine/LightDirectionalTemplate.h"
#include "Engine/LightPointComponent.h"
#include "Engine/LightPointTemplate.h"
#include "Engine/LinesComponent.h"
#include "Engine/MusicComponent.h"
#include "Engine/PhysicsComponent.h"
#include "Engine/PhysicsSceneComponent.h"
#include "Engine/PhysicsTemplate.h"
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
#include "Engine/SpriteTemplate.h"
#include "Engine/StaticMeshComponent.h"
#include "Engine/StaticMeshTemplate.h"
#include "Engine/TablesReloadedEvent.h"
#include "Engine/TemplateManager.h"
#include "Engine/TextComponent.h"
#include "Engine/TextTemplate.h"
#include "Engine/TransformComponent.h"
#include "Engine/TransformTemplate.h"
#include "Engine/UserComponent.h"
#include "Engine/UserMapComponent.h"
#include "Engine/UUIDComponent.h"
#include "Engine/UUIDMapComponent.h"
#include "Engine/VersionComponent.h"
#include "Engine/VisibilityComponent.h"
#include "Engine/VisibilityTemplate.h"
#include "Serialize/Visitor.h"

template<>
void Visitor::ReadCustom(ecs::NameComponent& value) const
{
}
template<>
void Visitor::WriteCustom(const ecs::NameComponent& value)
{
}

template<>
void Visitor::ReadCustom(ecs::ReplicationComponent& value) const
{
}
template<>
void Visitor::WriteCustom(const ecs::ReplicationComponent& value)
{
}

void eng::RegisterClientComponents(ecs::EntityWorld& world)
{
	world.RegisterComponent<eng::DynamicMeshComponent>();
	world.RegisterComponent<eng::FrameBufferComponent>();
	world.RegisterComponent<eng::light::AmbientComponent>();
	world.RegisterComponent<eng::light::AmbientTemplate>();
	world.RegisterComponent<eng::light::DirectionalComponent>();
	world.RegisterComponent<eng::light::DirectionalTemplate>();
	world.RegisterComponent<eng::light::PointComponent>();
	world.RegisterComponent<eng::light::PointTemplate>();
	world.RegisterComponent<eng::LinesComponent>();
	world.RegisterComponent<eng::MusicComponent>();
	world.RegisterComponent<eng::SavegameComponent>();
	world.RegisterComponent<eng::settings::AudioComponent>();
	world.RegisterComponent<eng::settings::DebugComponent>();
	world.RegisterComponent<eng::settings::GameplayComponent>();
	world.RegisterComponent<eng::settings::WindowComponent>();
	world.RegisterComponent<eng::sound::ObjectComponent>();
	world.RegisterComponent<eng::sound::RandomBufferComponent>();
	world.RegisterComponent<eng::sound::RandomComponent>();
	world.RegisterComponent<eng::sound::RandomRequestComponent>();
	world.RegisterComponent<eng::sound::SequenceBufferComponent>();
	world.RegisterComponent<eng::sound::SequenceComponent>();
	world.RegisterComponent<eng::sound::SequenceRequestComponent>();
	world.RegisterComponent<eng::sound::SingleRequestComponent>();
	world.RegisterComponent<eng::VisibilityComponent>();
	world.RegisterComponent<eng::VisibilityTemplate>();
	world.RegisterEvent<eng::application::CloseEvent>();
	world.RegisterEvent<eng::TablesReloadedEvent>();

	// templates
	{
		auto& manager = world.WriteResource<eng::TemplateManager>();
		manager.RegisterComponent<eng::light::AmbientTemplate>();
		manager.RegisterComponent<eng::light::DirectionalTemplate>();
		manager.RegisterComponent<eng::light::PointTemplate>();
		manager.RegisterComponent<eng::VisibilityTemplate>();
	}
}

void eng::RegisterServerComponents(ecs::EntityWorld& world)
{
	world.RegisterComponent<net::UserMapComponent>();
}

void eng::RegisterSharedComponents(ecs::EntityWorld& world)
{
	world.RegisterComponent<ecs::ReplicationComponent>();
	world.RegisterComponent<eng::ActiveComponent>();
	world.RegisterComponent<eng::AssetComponent>();
	world.RegisterComponent<eng::CameraComponent>();
	world.RegisterComponent<eng::CameraTemplate>();
	world.RegisterComponent<eng::EditorComponent>();
	world.RegisterComponent<eng::FlipbookComponent>();
	world.RegisterComponent<eng::FlipbookTemplate>();
	world.RegisterComponent<eng::level::DirectoryComponent>();
	world.RegisterComponent<eng::level::EntityComponent>();
	world.RegisterComponent<eng::level::LoadedComponent>();
	world.RegisterComponent<eng::level::LoadingComponent>();
	world.RegisterComponent<eng::PhysicsComponent>();
	world.RegisterComponent<eng::PhysicsSceneComponent>();
	world.RegisterComponent<eng::PhysicsTemplate>();
	world.RegisterComponent<eng::RigidDynamicComponent>();
	world.RegisterComponent<eng::RigidStaticComponent>();
	world.RegisterComponent<eng::settings::LaunchComponent>();
	world.RegisterComponent<eng::SpriteComponent>();
	world.RegisterComponent<eng::SpriteTemplate>();
	world.RegisterComponent<eng::StaticMeshComponent>();
	world.RegisterComponent<eng::StaticMeshTemplate>();
	world.RegisterComponent<eng::TextComponent>();
	world.RegisterComponent<eng::TextTemplate>();
	world.RegisterComponent<eng::TransformComponent>();
	world.RegisterComponent<eng::TransformTemplate>();
	world.RegisterComponent<eng::UUIDComponent>();
	world.RegisterComponent<eng::UUIDMapComponent>();
	world.RegisterComponent<eng::VersionComponent>();
	world.RegisterComponent<net::UserComponent>();
	world.RegisterEvent<eng::level::LoadEvent>();

	// templates
	{
		auto& manager = world.WriteResource<eng::TemplateManager>();
		manager.RegisterComponent<eng::CameraTemplate>();
		manager.RegisterComponent<eng::FlipbookTemplate>();
		manager.RegisterComponent<eng::PhysicsTemplate>();
		manager.RegisterComponent<eng::SpriteTemplate>();
		manager.RegisterComponent<eng::StaticMeshTemplate>();
		manager.RegisterComponent<eng::TextTemplate>();
		manager.RegisterComponent<eng::TransformTemplate>();
	}
}
