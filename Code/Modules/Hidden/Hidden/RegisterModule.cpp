#include "HiddenPCH.h"
#include "Hidden/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Engine/TableHeadmaster.h"
#include "Hidden/HiddenCountComponent.h"
#include "Hidden/HiddenCountSystem.h"
#include "Hidden/HiddenDebugSettingsSingleton.h"
#include "Hidden/HiddenGroupComponent.h"
#include "Hidden/HiddenObjectComponent.h"
#include "Hidden/HiddenPhysicsSystem.h"
#include "Hidden/HiddenRevealComponent.h"
#include "Hidden/HiddenRevealSystem.h"
#include "Hidden/HiddenSaveLoadSystem.h"
#include "Hidden/HiddenSoundSystem.h"
#include "Hidden/HiddenSpriteSystem.h"
#include "Hidden/HiddenTrackerSystem.h"
#include "Hidden/HiddenVFXComponent.h"
#include "Hidden/HiddenVFXSystem.h"

void hidden::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// components
	{
		entityWorld.RegisterComponent<hidden::CountComponent>();
		entityWorld.RegisterComponent<hidden::GroupComponent>();
		entityWorld.RegisterComponent<hidden::ObjectComponent>();
		entityWorld.RegisterComponent<hidden::RevealComponent>();
		entityWorld.RegisterComponent<hidden::VFXComponent>();
	}

	// singletons
	{
		entityWorld.RegisterSingleton<hidden::settings::DebugSingleton>();
	}

	// systems
	{
		entityWorld.RegisterSystem<hidden::CountSystem>();
		entityWorld.RegisterSystem<hidden::PhysicsSystem>();
		entityWorld.RegisterSystem<hidden::RevealSystem>();
		entityWorld.RegisterSystem<hidden::SaveLoadSystem>();
		entityWorld.RegisterSystem<hidden::SoundSystem>();
		entityWorld.RegisterSystem<hidden::SpriteSystem>();
		entityWorld.RegisterSystem<hidden::TrackerSystem>();
		entityWorld.RegisterSystem<hidden::VFXSystem>();
	}

	// prototypes
	{
		auto& prototypeManager = entityWorld.WriteResource<eng::PrototypeManager>();
		prototypeManager.Register<hidden::CountComponent>();
		prototypeManager.Register<hidden::GroupComponent>();
		prototypeManager.Register<hidden::ObjectComponent>();
	}
}