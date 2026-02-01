#include "SharedHiddenPCH.h"
#include "SharedHidden/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/TypeRegistry.h"
#include "ECS/WorldView.h"
#include "SharedHidden/HiddenRevealedEvent.h"
#include "SharedHidden/HiddenSelectedEvent.h"

void shared::hidden::RegisterModule(ecs::EntityWorld& world)
{
	auto& registry = world.WriteResource<ecs::TypeRegistry>();

	// events
	{
		world.RegisterEvent<shared::hidden::RevealedEvent>();
		registry.RegisterEvent<shared::hidden::RevealedEvent>();
		world.RegisterEvent<shared::hidden::SelectedEvent>();
		registry.RegisterEvent<shared::hidden::SelectedEvent>();
	}
}