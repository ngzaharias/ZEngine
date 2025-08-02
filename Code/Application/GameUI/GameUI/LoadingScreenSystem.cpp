#include "GameUIPCH.h"
#include "GameUI/LoadingScreenSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelComponents.h"
#include "Engine/UIManager.h"
#include "GameUI/DCLoadingScreen.h"

namespace
{
	const str::Name strLoadingScreen_xaml = NAME("LoadingScreen.xaml");
}

void gui::LoadingScreenSystem::Update(World& world, const GameTime& gameTime)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<eng::level::LoadingComponent>>())
	{
		const auto& loadingComponent = world.ReadComponent<eng::level::LoadingComponent>(entity);
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.CreateWidget(strLoadingScreen_xaml);

		auto& dataContext = uiManager.WriteDataContext<gui::DCLoadingScreen>(strLoadingScreen_xaml);
		dataContext.SetIsSplash(loadingComponent.m_IsSplash);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<eng::level::LoadingComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strLoadingScreen_xaml);
	}
}
