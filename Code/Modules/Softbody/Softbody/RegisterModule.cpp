#include "SoftbodyPCH.h"
#include "Softbody/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Softbody/SoftbodyChainSystem.h"
#include "Softbody/SoftbodyComponents.h"

void softbody::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// systems
	{
		entityWorld.RegisterSystem<softbody::ChainSystem>();
	}

	// components
	{
		entityWorld.RegisterComponent<softbody::ChainComponent>();
	}	
	
	// prototypes
	{
		auto& prototypeManager = entityWorld.WriteResource<eng::PrototypeManager>();
		prototypeManager.Register<softbody::ChainComponent>();
	}
}