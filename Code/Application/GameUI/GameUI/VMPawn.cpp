#include "GameUIPCH.h"
#include "GameUI/VMPawn.h"

#include <NsCore/ReflectionImplement.h>
#include <NsGui/ObservableCollection.h>

gui::VMPawn::VMPawn(const ecs::Entity& entity, const str::String& name)
	: m_Entity(entity)
	, m_Name(name)
{
}

Noesis::ObservableCollection<gui::VMAbility>* gui::VMPawn::GetAbilities() const
{
	return m_Abilities;
}

void gui::VMPawn::SetAbilities(Noesis::Ptr<Noesis::ObservableCollection<gui::VMAbility>> value)
{
	m_Abilities = value;
	OnPropertyChanged("Abilities");
}

NS_IMPLEMENT_REFLECTION(gui::VMPawn)
{
	NsProp("Name", &gui::VMPawn::GetName);
	NsProp("Abilities", &gui::VMPawn::GetAbilities);
}