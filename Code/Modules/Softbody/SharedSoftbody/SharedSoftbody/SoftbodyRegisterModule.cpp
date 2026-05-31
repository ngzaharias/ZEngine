#include "SoftbodyPCH.h"
#include "SharedSoftbody/SoftbodyRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TemplateManager.h"
#include "SharedSoftbody/SoftbodyChainComponent.h"
#include "SharedSoftbody/SoftbodyChainSystem.h"
#include "SharedSoftbody/SoftbodyChainTemplate.h"

void shared::softbody::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<shared::softbody::ChainComponent>();
	world.RegisterComponent<shared::softbody::ChainTemplate>();
	world.RegisterSystem<shared::softbody::ChainSystem>();

	// templates
	{
		auto& manager = world.WriteResource<eng::TemplateManager>();
		manager.RegisterComponent<shared::softbody::ChainTemplate>();
	}
}