#include "GameUIPCH.h"
#include "GameUI/DCHud.h"

#include "ECS/EntityWorld.h"
#include "GameUI/VMPawn.h"
#include "Tactics/TacticsAbilityPreviewEvent.h"

#include <NsCore/Delegate.h>
#include <NsCore/ReflectionImplement.h>

gui::DCHud::DCHud()
{
	m_AbilityPreviewCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCHud::OnAbilityPreviewCommand));
}

gui::DCHud::~DCHud()
{
}

gui::VMPawn* gui::DCHud::GetSelectedPawn() const
{
	return m_SelectedPawn;
}

void gui::DCHud::SetSelectedPawn(Noesis::Ptr<gui::VMPawn> value)
{
	m_SelectedPawn = value;
	OnPropertyChanged("SelectedPawn");
}

void gui::DCHud::OnAbilityPreviewCommand(Noesis::BaseComponent* param)
{
	if (const auto* vmAbility = Noesis::DynamicCast<gui::VMAbility*>(param))
	{
		auto& data = m_EntityWorld->AddEvent<tactics::AbilityPreviewEvent>();
		data.m_Entity = m_SelectedPawn->GetEntity();
		data.m_Ability = vmAbility->GetName();
	}
}

NS_IMPLEMENT_REFLECTION(gui::DCHud)
{
	NsProp("SelectedPawn", &gui::DCHud::GetSelectedPawn);
	NsProp("AbilityPreviewCommand", &gui::DCHud::GetAbilityPreviewCommand);
}