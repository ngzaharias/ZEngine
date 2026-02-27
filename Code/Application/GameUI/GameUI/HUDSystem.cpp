#include "GameUIPCH.h"
#include "GameUI/HUDSystem.h"

#include "Core/Name.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/UIManager.h"
#include "GameUI/DCHUD.h"
#include "GameUI/HUDComponent.h"
#include "Tactics/TacticsAbilityComponent.h"
#include "Tactics/TacticsSelectedComponent.h"

#include <NsGui/ObservableCollection.h>

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
	}

	for (auto&& view : world.Query<ecs::query::Removed<gui::HUDComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strHUD_xaml);
	}

	using SelectedQuery = ecs::query
		::Added<
		tactics::SelectedComponent>
		::Include<
		ecs::NameComponent,
		tactics::AbilityComponent>;
	for (auto&& view : world.Query<SelectedQuery>())
	{
		const auto& nameComponent = view.ReadRequired<ecs::NameComponent>();
		const auto& abilityComponent = view.ReadRequired<tactics::AbilityComponent>();
		auto& uiManager = world.WriteResource<eng::UIManager>();
		auto& hud = uiManager.WriteDataContext<gui::DCHud>(strHUD_xaml);

		auto abilities = Noesis::MakePtr<Noesis::ObservableCollection<gui::VMAbility>>();
		for (const str::Name& name : abilityComponent.m_Abilities)
		{
			auto ability = Noesis::MakePtr<gui::VMAbility>(name);
			abilities->Add(ability);
		}

		auto pawn = Noesis::MakePtr<gui::VMPawn>(nameComponent.m_Name);
		pawn->SetAbilities(abilities);
		hud.SetSelectedPawn(pawn);
	}

	for (auto&& view : world.Query<ecs::query::Removed<tactics::SelectedComponent>>())
	{
		if (world.HasAny<ecs::query::Include<tactics::SelectedComponent>>())
			continue;

		auto& uiManager = world.WriteResource<eng::UIManager>();
		auto& hud = uiManager.WriteDataContext<gui::DCHud>(strHUD_xaml);
		hud.SetSelectedPawn(nullptr);
	}
}
