#include "AssetPCH.h"
#include "AssetEditor/AssetRegisterModule.h"

#include "AssetEditor/AssetBrowserComponent.h"
#include "AssetEditor/AssetBrowserOpenEvent.h"
#include "AssetEditor/AssetBrowserSystem.h"
#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"

void editor::assets::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<editor::assets::BrowserComponent>();
	world.RegisterEvent<editor::assets::BrowserOpenEvent>();
	world.RegisterSystem<editor::assets::BrowserSystem>();
}