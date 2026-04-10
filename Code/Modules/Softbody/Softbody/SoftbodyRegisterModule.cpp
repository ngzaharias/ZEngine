#include "SoftbodyPCH.h"
#include "Softbody/SoftbodyRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TemplateManager.h"
#include "Softbody/SoftbodyChainComponent.h"
#include "Softbody/SoftbodyChainSystem.h"
#include "Softbody/SoftbodyChainTemplate.h"

void softbody::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<softbody::ChainComponent>();
	world.RegisterComponent<softbody::ChainTemplate>();
	world.RegisterSystem<softbody::ChainSystem>();

	// templates
	{
		auto& manager = world.WriteResource<eng::TemplateManager>();
		manager.RegisterComponent<softbody::ChainTemplate>();
	}
}