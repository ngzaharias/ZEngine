#include "SoftbodyPCH.h"
#include "Softbody/SoftbodyRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Softbody/SoftbodyChainComponent.h"
#include "Softbody/SoftbodyChainSystem.h"

void softbody::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<softbody::ChainComponent>();
	}	
	
	// systems
	{
		world.RegisterSystem<softbody::ChainSystem>();
	}

	// prototypes
	{
		auto& manager = world.WriteResource<eng::PrototypeManager>();
		manager.RegisterComponent<softbody::ChainComponent>();
	}
}