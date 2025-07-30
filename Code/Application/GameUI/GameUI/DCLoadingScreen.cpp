#include "GameUIPCH.h"
#include "GameUI/DCLoadingScreen.h"

#include "ECS/EntityWorld.h"
#include "Engine/SettingsComponents.h"

#include <NsCore/ReflectionImplement.h>

gui::DCLoadingScreen::DCLoadingScreen()
{
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

NS_IMPLEMENT_REFLECTION(gui::DCLoadingScreen)
{
	NsProp("Progress", &gui::DCLoadingScreen::GetProgress, &gui::DCLoadingScreen::SetProgress);
}