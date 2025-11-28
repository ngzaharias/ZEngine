#include "GameUIPCH.h"
#include "GameUI/LoadingSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelLoadingComponent.h"
#include "Engine/UIManager.h"
#include "GameUI/DCLoadingScreen.h"
#include "GameUI/HintTable.h"
#include "GameUI/LoadingComponents.h"

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

		const auto& hints = world.ReadResource<gui::HintTable>();
		const auto& hint = hints.GetRandom();
		auto& dcLoading = uiManager.WriteDataContext<gui::DCLoadingScreen>(widget);
		dcLoading.SetHint(hint.m_Text.c_str());
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<eng::level::LoadingComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strLoading_xaml);
		uiManager.DestroyWidget(strSplash_xaml);
	}

	// Events
	for (const auto& request : world.Events<gui::loading::CloseRequest>())
	{
		for (const ecs::Entity& levelEntity : world.Query<ecs::query::Include<eng::level::LoadingComponent>>())
		{
			auto& loadingComponent = world.WriteComponent<eng::level::LoadingComponent>(levelEntity);
			loadingComponent.m_FadeOutTimer = loadingComponent.m_FadeOutTime;
		}
	}
}
