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

bool gui::DCLoadingScreen::GetIsSplash() const
{
	return m_IsSplash;
}

void gui::DCLoadingScreen::SetIsSplash(bool value)
{
	if (m_IsSplash != value)
	{
		m_IsSplash = value;
		OnPropertyChanged("IsSplash");
	}
}

NS_IMPLEMENT_REFLECTION(gui::DCLoadingScreen)
{
	NsProp("Progress", &gui::DCLoadingScreen::GetProgress, &gui::DCLoadingScreen::SetProgress);
	NsProp("IsSplash", &gui::DCLoadingScreen::GetIsSplash, &gui::DCLoadingScreen::SetIsSplash);
}