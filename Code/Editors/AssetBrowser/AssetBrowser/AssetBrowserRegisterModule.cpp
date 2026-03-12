#include "AssetBrowserPCH.h"
#include "AssetBrowser/AssetBrowserRegisterModule.h"

#include "AssetBrowser/AssetBrowserOpenWindowEvent.h"
#include "AssetBrowser/AssetBrowserWindowComponent.h"
#include "AssetBrowser/AssetBrowserWindowSystem.h"
#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"

void editor::assets::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<editor::assets::WindowComponent>();
	world.RegisterEvent<editor::assets::OpenWindowEvent>();
	world.RegisterSystem<editor::assets::WindowSystem>();
}