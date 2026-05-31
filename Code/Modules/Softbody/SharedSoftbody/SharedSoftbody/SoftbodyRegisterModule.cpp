#include "SoftbodyPCH.h"
#include "SharedSoftbody/SoftbodyRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TemplateManager.h"
#include "SharedSoftbody/SoftbodyChainComponent.h"
#include "SharedSoftbody/SoftbodyChainSystem.h"
#include "SharedSoftbody/SoftbodyChainTemplate.h"

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