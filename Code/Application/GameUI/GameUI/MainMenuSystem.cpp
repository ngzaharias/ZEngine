#include "GameUIPCH.h"
#include "GameUI/MainMenuSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ApplicationComponents.h"
#include "Engine/LevelComponents.h"
#include "Engine/UIManager.h"
#include "GameUI/DCMainMenu.h"
#include "GameUI/MainMenuComponent.h"
#include "GameUI/SettingsComponents.h"

namespace
{
	const str::Name strMainMenu = NAME("MainMenu.xaml");
}

void gui::main_menu::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<gui::main_menu::MenuComponent>>())
	{
		const auto& menuComponent = world.ReadComponent<gui::main_menu::MenuComponent>(entity);
		auto& uiManager = world.WriteResource<eng::UIManager>();

		auto& dataContext = uiManager.WriteDataContext<gui::DCMainMenu>(strMainMenu);
		dataContext.SetNewGameLevel(menuComponent.m_NewGame);

		uiManager.CreateWidget(strMainMenu);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<gui::main_menu::MenuComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strMainMenu);
	}
}