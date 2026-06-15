#include "AssetPCH.h"
#include "AssetEditor/AssetRegisterModule.h"

#include "AssetEditor/AssetBrowserComponent.h"
#include "AssetEditor/AssetBrowserOpenEvent.h"
#include "AssetEditor/AssetBrowserSystem.h"
#include "AssetEditor/AssetImporterComponent.h"
#include "AssetEditor/AssetImporterSystem.h"
#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"

void editor::assets::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<editor::assets::BrowserComponent>();
	world.RegisterComponent<editor::assets::ImporterComponent>();
	world.RegisterEvent<editor::assets::BrowserOpenEvent>();
	world.RegisterSystem<editor::assets::BrowserSystem>();
	world.RegisterSystem<editor::assets::ImporterSystem>();
}