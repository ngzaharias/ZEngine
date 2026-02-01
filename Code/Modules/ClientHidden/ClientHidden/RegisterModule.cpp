#include "ClientHiddenPCH.h"
#include "ClientHidden/RegisterModule.h"

#include "ClientHidden/HiddenCountComponent.h"
#include "ClientHidden/HiddenCountSystem.h"
#include "ClientHidden/HiddenDebugSingleton.h"
#include "ClientHidden/HiddenGroupComponent.h"
#include "ClientHidden/HiddenObjectComponent.h"
#include "ClientHidden/HiddenPhysicsSystem.h"
#include "ClientHidden/HiddenRevealComponent.h"
#include "ClientHidden/HiddenRevealSystem.h"
#include "ClientHidden/HiddenSaveLoadSystem.h"
#include "ClientHidden/HiddenSoundSystem.h"
#include "ClientHidden/HiddenSpriteSystem.h"
#include "ClientHidden/HiddenTrackerSystem.h"
#include "ClientHidden/HiddenVFXComponent.h"
#include "ClientHidden/HiddenVFXSystem.h"
#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Engine/TableHeadmaster.h"

void client::hidden::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<client::hidden::CountComponent>();
		world.RegisterComponent<client::hidden::GroupComponent>();
		world.RegisterComponent<client::hidden::ObjectComponent>();
		world.RegisterComponent<client::hidden::RevealComponent>();
		world.RegisterComponent<client::hidden::VFXComponent>();
	}

	// singletons
	{
		world.RegisterSingleton<client::hidden::DebugSingleton>();
	}

	// systems
	{
		world.RegisterSystem<client::hidden::CountSystem>();
		world.RegisterSystem<client::hidden::PhysicsSystem>();
		world.RegisterSystem<client::hidden::RevealSystem>();
		world.RegisterSystem<client::hidden::SaveLoadSystem>();
		world.RegisterSystem<client::hidden::SoundSystem>();
		world.RegisterSystem<client::hidden::SpriteSystem>();
		world.RegisterSystem<client::hidden::TrackerSystem>();
		world.RegisterSystem<client::hidden::VFXSystem>();
	}

	// prototypes
	{
		auto& manager = world.WriteResource<eng::PrototypeManager>();
		manager.Register<client::hidden::CountComponent>();
		manager.Register<client::hidden::GroupComponent>();
		manager.Register<client::hidden::ObjectComponent>();
	}
}