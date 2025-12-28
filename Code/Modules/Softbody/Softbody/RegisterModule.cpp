#include "SoftbodyPCH.h"
#include "Softbody/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Softbody/SoftbodyChainComponent.h"
#include "Softbody/SoftbodyChainSystem.h"

void softbody::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// components
	{
		entityWorld.RegisterComponent<softbody::ChainComponent>();
	}	
	
	// systems
	{
		entityWorld.RegisterSystem<softbody::ChainSystem>();
	}

	// prototypes
	{
		auto& prototypeManager = entityWorld.WriteResource<eng::PrototypeManager>();
		prototypeManager.Register<softbody::ChainComponent>();
	}
}