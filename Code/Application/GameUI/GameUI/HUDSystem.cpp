#include "GameUIPCH.h"
#include "GameUI/HUDSystem.h"

#include "Core/Name.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/UIManager.h"
#include "GameUI/DCHUD.h"
#include "GameUI/HUDComponent.h"

namespace
{
	const str::Name strHUD_xaml = NAME("HUD.xaml");
}

void gui::HUDSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using AddedQuery = ecs::query
		::Added<gui::HUDComponent>
		::Include<const gui::HUDComponent>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const auto& counter = view.ReadRequired<gui::HUDComponent>();
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.CreateWidget(strHUD_xaml);

		gui::DCHud::World other = world;
		auto& dataContext = uiManager.WriteDataContext<gui::DCHud>(strHUD_xaml);
		dataContext.Initialise(other);
	}

	for (auto&& view : world.Query<ecs::query::Removed<gui::HUDComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strHUD_xaml);
	}
}
