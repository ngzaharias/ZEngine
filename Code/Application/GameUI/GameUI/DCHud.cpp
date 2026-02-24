#include "GameUIPCH.h"
#include "GameUI/DCHud.h"

#include "GameUI/VMAbility.h"
#include "Tactics/TacticsAbilityTable.h"

#include <NsCore/ReflectionImplement.h>
#include <NsGui/ObservableCollection.h>

gui::DCHud::DCHud()
{
	m_Abilities = Noesis::MakePtr<Noesis::ObservableCollection<gui::VMAbility>>();
}

gui::DCHud::~DCHud()
{
}

void gui::DCHud::Initialise(World& world)
{
	PROFILE_FUNCTION();

	const auto& abilities = world.ReadResource<tactics::AbilityTable>();

	m_Abilities->Clear();
	for (auto&& [key, value] : abilities.GetObjectMap())
	{
		auto ability = Noesis::MakePtr<gui::VMAbility>(key);
		m_Abilities->Add(ability);
	}
}

Noesis::ObservableCollection<gui::VMAbility>* gui::DCHud::GetAbilities() const
{
	return m_Abilities;
}

void gui::DCHud::SetAbilities(Noesis::Ptr<Noesis::ObservableCollection<gui::VMAbility>> value)
{
	if (m_Abilities != value)
	{
		m_Abilities = value;
		OnPropertyChanged("Abilities");
	}
}

NS_IMPLEMENT_REFLECTION(gui::DCHud)
{
	NsProp("Abilities", &gui::DCHud::GetAbilities);
}