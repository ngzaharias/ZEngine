#include "GameUIPCH.h"
#include "GameUI/LoadingScreenSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelComponents.h"
#include "Engine/UIManager.h"

namespace
{
	const str::Name strLoadingScreen_xaml = NAME("LoadingScreen.xaml");
}

void gui::LoadingScreenSystem::Update(World& world, const GameTime& gameTime)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<eng::level::LoadingComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.CreateWidget(strLoadingScreen_xaml);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<eng::level::LoadingComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strLoadingScreen_xaml);
	}
}
