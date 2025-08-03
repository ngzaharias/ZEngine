#include "GameUIPCH.h"
#include "GameUI/LoadingSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelComponents.h"
#include "Engine/UIManager.h"
#include "GameUI/DCLoadingScreen.h"

namespace
{
	const str::Name strLoading_xaml = NAME("Loading.xaml");
	const str::Name strSplash_xaml = NAME("Splash.xaml");
}

void gui::loading::LoadingSystem::Update(World& world, const GameTime& gameTime)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<eng::level::LoadingComponent>>())
	{
		const auto& loadingComponent = world.ReadComponent<eng::level::LoadingComponent>(entity);
		auto& uiManager = world.WriteResource<eng::UIManager>();
		const str::Name& widget = !loadingComponent.m_IsSplash
			? strLoading_xaml
			: strSplash_xaml;
		uiManager.CreateWidget(widget);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<eng::level::LoadingComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strLoading_xaml);
		uiManager.DestroyWidget(strSplash_xaml);
	}
}
