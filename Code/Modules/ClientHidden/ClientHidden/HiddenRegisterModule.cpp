#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenRegisterModule.h"

#include "ClientHidden/HiddenCountComponent.h"
#include "ClientHidden/HiddenCountSystem.h"
#include "ClientHidden/HiddenCountTemplate.h"
#include "ClientHidden/HiddenDebugComponent.h"
#include "ClientHidden/HiddenGroupComponent.h"
#include "ClientHidden/HiddenGroupTemplate.h"
#include "ClientHidden/HiddenObjectComponent.h"
#include "ClientHidden/HiddenObjectTemplate.h"
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
#include "Engine/TableHeadmaster.h"
#include "Engine/TemplateManager.h"

void client::hidden::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<client::hidden::CountComponent>();
	world.RegisterComponent<client::hidden::CountTemplate>();
	world.RegisterComponent<client::hidden::DebugComponent>();
	world.RegisterComponent<client::hidden::GroupComponent>();
	world.RegisterComponent<client::hidden::GroupTemplate>();
	world.RegisterComponent<client::hidden::ObjectComponent>();
	world.RegisterComponent<client::hidden::ObjectTemplate>();
	world.RegisterComponent<client::hidden::RevealComponent>();
	world.RegisterComponent<client::hidden::VFXComponent>();
	world.RegisterSystem<client::hidden::CountSystem>();
	world.RegisterSystem<client::hidden::PhysicsSystem>();
	world.RegisterSystem<client::hidden::RevealSystem>();
	world.RegisterSystem<client::hidden::SaveLoadSystem>();
	world.RegisterSystem<client::hidden::SoundSystem>();
	world.RegisterSystem<client::hidden::SpriteSystem>();
	world.RegisterSystem<client::hidden::TrackerSystem>();
	world.RegisterSystem<client::hidden::VFXSystem>();

	// templates
	{
		auto& manager = world.WriteResource<eng::TemplateManager>();
		manager.RegisterComponent<client::hidden::CountTemplate>();
		manager.RegisterComponent<client::hidden::GroupTemplate>();
		manager.RegisterComponent<client::hidden::ObjectTemplate>();
	}
}