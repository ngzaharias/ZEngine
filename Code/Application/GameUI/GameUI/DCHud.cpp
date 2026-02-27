#include "GameUIPCH.h"
#include "GameUI/DCHud.h"

#include "GameUI/VMPawn.h"
#include "Tactics/TacticsAbilityTable.h"

#include <NsCore/ReflectionImplement.h>

gui::VMPawn* gui::DCHud::GetSelectedPawn() const
{
	return m_SelectedPawn;
}

void gui::DCHud::SetSelectedPawn(Noesis::Ptr<gui::VMPawn> value)
{
	m_SelectedPawn = value;
	OnPropertyChanged("SelectedPawn");
}

NS_IMPLEMENT_REFLECTION(gui::DCHud)
{
	NsProp("SelectedPawn", &gui::DCHud::GetSelectedPawn);
}