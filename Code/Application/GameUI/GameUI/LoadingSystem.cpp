#include "GameUIPCH.h"
#include "GameUI/LoadingSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelLoadingComponent.h"
#include "Engine/UIManager.h"
#include "GameUI/DCLoadingScreen.h"
#include "GameUI/HintTable.h"
#include "GameUI/LoadingCloseRequest.h"

namespace
{
	const str::Name strLoading_xaml = NAME("Loading.xaml");
	const str::Name strSplash_xaml = NAME("Splash.xaml");
}

void gui::loading::LoadingSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using AddedQuery = ecs::query
		::Added<eng::level::LoadingComponent>
		::Include<const eng::level::LoadingComponent>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const auto& loadingComponent = view.ReadRequired<eng::level::LoadingComponent>();
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

	for (auto&& view : world.Query<ecs::query::Removed<eng::level::LoadingComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strLoading_xaml);
		uiManager.DestroyWidget(strSplash_xaml);
	}

	// Events
	for (const auto& request : world.Events<gui::loading::CloseRequest>())
	{
		for (auto&& view : world.Query<ecs::query::Include<eng::level::LoadingComponent>>())
		{
			auto& loadingComponent = view.WriteRequired<eng::level::LoadingComponent>();
			loadingComponent.m_FadeOutTimer = loadingComponent.m_FadeOutTime;
		}
	}
}
