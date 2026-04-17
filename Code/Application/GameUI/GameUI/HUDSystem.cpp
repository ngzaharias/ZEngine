#include "GameUIPCH.h"
#include "GameUI/HUDSystem.h"

#include "Core/Name.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/UIManager.h"
#include "GameUI/DCHUD.h"
#include "GameUI/HUDTemplate.h"
#include "Tactics/TacticsAbilityTable.h"
#include "Tactics/TacticsPawnAbilitiesComponent.h"
#include "Tactics/TacticsPawnSelectedComponent.h"

#include <NsGui/ObservableCollection.h>

namespace
{
	const str::Name strHUD_xaml = NAME("HUD.xaml");
}

void gui::HUDSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using AddedQuery = ecs::query
		::Added<gui::HUDTemplate>
		::Include<const gui::HUDTemplate>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const auto& counter = view.ReadRequired<gui::HUDTemplate>();
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.CreateWidget(strHUD_xaml);
	}

	using RemovedHudQuery = ecs::query
		::Condition<ecs::Alive, ecs::Dead>
		::Removed<gui::HUDTemplate>;
	for (auto&& view : world.Query<RemovedHudQuery>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strHUD_xaml);
	}

	using SelectedQuery = ecs::query
		::Added<
		tactics::PawnSelectedComponent>
		::Include<
		const ecs::NameComponent,
		const tactics::PawnAbilitiesComponent>;
	for (auto&& view : world.Query<SelectedQuery>())
	{
		const auto& nameComponent = view.ReadRequired<ecs::NameComponent>();
		const auto& abilityComponent = view.ReadRequired<tactics::PawnAbilitiesComponent>();
		const auto& abilityTable = world.ReadResource<tactics::AbilityTable>();
		auto& uiManager = world.WriteResource<eng::UIManager>();
		auto& hud = uiManager.WriteDataContext<gui::DCHud>(strHUD_xaml);

		auto vmAbilities = Noesis::MakePtr<Noesis::ObservableCollection<gui::VMAbility>>();
		for (const str::Name& name : abilityComponent.m_Abilities)
		{
			const auto& ability = abilityTable.GetObject(name);
			auto vmAbility = Noesis::MakePtr<gui::VMAbility>(name, ability);
			vmAbilities->Add(vmAbility);
		}

		auto vmPawn = Noesis::MakePtr<gui::VMPawn>(view, nameComponent.m_Name);
		vmPawn->SetAbilities(vmAbilities);
		hud.SetSelectedPawn(vmPawn);
	}

	using RemovedSelectedQuery = ecs::query
		::Condition<ecs::Alive, ecs::Dead>
		::Removed<tactics::PawnSelectedComponent>;
	for (auto&& view : world.Query<RemovedSelectedQuery>())
	{
		if (world.HasAny<ecs::query::Include<tactics::PawnSelectedComponent>>())
			continue;

		auto& uiManager = world.WriteResource<eng::UIManager>();
		auto& hud = uiManager.WriteDataContext<gui::DCHud>(strHUD_xaml);
		hud.SetSelectedPawn(nullptr);
	}
}
