#include "IconsPCH.h"
#include "Icons/IconsRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Icons/IconsLoadSystem.h"

void icon::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterSystem<icon::LoadSystem>();
}