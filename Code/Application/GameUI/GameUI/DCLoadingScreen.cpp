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

const char* gui::DCLoadingScreen::GetHint() const
{
	return m_Hint.c_str();
}

void gui::DCLoadingScreen::SetHint(const char* value)
{
	if (m_Hint != value)
	{
		m_Hint = value;
		OnPropertyChanged("Hint");
	}
}

void gui::DCLoadingScreen::OnCloseCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::loading::CloseRequest>();
}

NS_IMPLEMENT_REFLECTION(gui::DCLoadingScreen)
{
	NsProp("Hint", &gui::DCLoadingScreen::GetHint, &gui::DCLoadingScreen::SetHint);

	NsProp("CloseCommand", &gui::DCLoadingScreen::GetCloseCommand);
}