#include "GameUIPCH.h"
#include "GameUI/DCLevelComplete.h"

#include "ECS/EntityWorld.h"
#include "GameUI/LevelCompleteComponents.h"

#include <NsCore/ReflectionImplement.h>

gui::DCLevelComplete::DCLevelComplete()
{
	m_ExitGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCLevelComplete::OnExitGameCommand));
	m_ExitToMenuCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCLevelComplete::OnExitToMenuCommand));
	m_ResetGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCLevelComplete::OnResetGameCommand));
}

gui::DCLevelComplete::~DCLevelComplete()
{
}

void gui::DCLevelComplete::OnExitGameCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::level_complete::ExitGameRequest>();
}

void gui::DCLevelComplete::OnExitToMenuCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::level_complete::ExitToMenuRequest>();
}

void gui::DCLevelComplete::OnResetGameCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::level_complete::ResetGameRequest>();
}

NS_IMPLEMENT_REFLECTION(gui::DCLevelComplete)
{
	NsProp("ExitGameCommand", &gui::DCLevelComplete::GetExitGameCommand);
	NsProp("ExitToMenuCommand", &gui::DCLevelComplete::GetExitToMenuCommand);
	NsProp("ResetGameCommand", &gui::DCLevelComplete::GetResetGameCommand);
}