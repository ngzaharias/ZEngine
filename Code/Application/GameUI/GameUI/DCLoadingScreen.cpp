#include "GameUIPCH.h"
#include "GameUI/DCLoadingScreen.h"

#include "ECS/EntityWorld.h"
#include "Engine/SettingsComponents.h"
#include "GameUI/LoadingComponents.h"

#include <NsCore/ReflectionImplement.h>

gui::DCLoadingScreen::DCLoadingScreen()
{
	m_CloseCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCLoadingScreen::OnCloseCommand));
}

gui::DCLoadingScreen::~DCLoadingScreen()
{
}

int32 gui::DCLoadingScreen::GetProgress() const
{
	return m_Progress;
}

void gui::DCLoadingScreen::SetProgress(int32 value)
{
	if (m_Progress != value)
	{
		m_Progress = value;
		OnPropertyChanged("Progress");
	}
}

void gui::DCLoadingScreen::OnCloseCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::loading::CloseRequest>();
}

NS_IMPLEMENT_REFLECTION(gui::DCLoadingScreen)
{
	NsProp("Progress", &gui::DCLoadingScreen::GetProgress, &gui::DCLoadingScreen::SetProgress);

	NsProp("CloseCommand", &gui::DCLoadingScreen::GetCloseCommand);
}