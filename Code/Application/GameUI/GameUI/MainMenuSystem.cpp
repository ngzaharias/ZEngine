#include "GameUIPCH.h"
#include "GameUI/MainMenuSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ApplicationCloseRequest.h"
#include "Engine/LevelLoadRequest.h"
#include "Engine/UIManager.h"
#include "Engine/VersionComponent.h"
#include "GameUI/DCMainMenu.h"
#include "GameUI/MainMenuContinueGameRequest.h"
#include "GameUI/MainMenuExitGameRequest.h"
#include "GameUI/MainMenuLoadGameRequest.h"
#include "GameUI/MainMenuNewGameRequest.h"
#include "GameUI/MainMenuWindowComponent.h"

namespace
{
	const str::Name strMainMenu_xaml = NAME("MainMenu.xaml");
}

void gui::main_menu::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	using AddedQuery = ecs::query
		::Added<gui::main_menu::WindowComponent>
		::Include<gui::main_menu::WindowComponent>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.CreateWidget(strMainMenu_xaml);

		const auto& versionComponent = world.ReadSingleton<eng::VersionSingleton>();
		const auto& menuComponent = view.ReadRequired<gui::main_menu::WindowComponent>();
		auto& dataContext = uiManager.WriteDataContext<gui::DCMainMenu>(strMainMenu_xaml);
		dataContext.SetNewGameLevel(menuComponent.m_NewGame);
		dataContext.SetVersion(versionComponent.m_Version);
	}

	for (auto&& view : world.Query<ecs::query::Removed<gui::main_menu::WindowComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strMainMenu_xaml);
	}

	//////////////////////////////////////////////////////////////////////////
	// Events

	if (world.HasAny<gui::main_menu::ContinueGameRequest>())
	{
	}

	if (world.HasAny<gui::main_menu::ExitGameRequest>())
	{
		world.AddEvent<eng::application::CloseRequest>();
	}

	if (world.HasAny<gui::main_menu::LoadGameRequest>())
	{
	}

	for (const auto& request : world.Events<gui::main_menu::NewGameRequest>())
	{
		world.AddEvent<eng::level::LoadRequest>(request.m_Level);
	}
}