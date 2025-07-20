#include "GameUIPCH.h"
#include "GameUI/DCSettingsMenu.h"

#include "ECS/EntityWorld.h"
#include "GameUI/SettingsMenuComponents.h"

#include <NsCore/ReflectionImplement.h>
#include <NsGui/ObservableCollection.h>

gui::VMMonitor::VMMonitor(const char* name) 
	: m_Name(name)
{
}

const char* gui::VMMonitor::GetName() const
{
	return m_Name.Str();
}

NS_IMPLEMENT_REFLECTION(gui::VMMonitor)
{
	NsProp("Name", &gui::VMMonitor::GetName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

gui::DCSettingsMenu::DCSettingsMenu()
{
	m_Monitors = *new Noesis::ObservableCollection<gui::VMMonitor>();
	m_Monitors->Add(Noesis::MakePtr<gui::VMMonitor>("800x600"));
	m_Monitors->Add(Noesis::MakePtr<gui::VMMonitor>("1920x1080"));

	m_CloseCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCSettingsMenu::OnCloseCommand));
}

float gui::DCSettingsMenu::GetEffectVolume() const
{
	return m_EffectVolume;
}

void gui::DCSettingsMenu::SetEffectVolume(float value)
{
	if (m_EffectVolume != value)
	{
		m_EffectVolume = value;
		OnPropertyChanged("EffectVolume");
	}
}

float gui::DCSettingsMenu::GetMasterVolume() const
{
	return m_MasterVolume;
}

void gui::DCSettingsMenu::SetMasterVolume(float value)
{
	if (m_MasterVolume != value)
	{
		m_MasterVolume = value;
		OnPropertyChanged("MasterVolume");
	}
}

float gui::DCSettingsMenu::GetMusicVolume() const
{
	return m_MusicVolume;
}

void gui::DCSettingsMenu::SetMusicVolume(float value)
{
	if (m_MusicVolume != value)
	{
		m_MusicVolume = value;
		OnPropertyChanged("MusicVolume");
	}
}

float gui::DCSettingsMenu::GetMoveSpeed() const
{
	return m_MoveSpeed;
}

void gui::DCSettingsMenu::SetMoveSpeed(float value)
{
	if (m_MoveSpeed != value)
	{
		m_MoveSpeed = value;
		OnPropertyChanged("MoveSpeed");
	}
}

float gui::DCSettingsMenu::GetZoomRate() const
{
	return m_ZoomRate;
}

void gui::DCSettingsMenu::SetZoomRate(float value)
{
	if (m_ZoomRate != value)
	{
		m_ZoomRate = value;
		OnPropertyChanged("ZoomRate");
	}
}

float gui::DCSettingsMenu::GetZoomSpeed() const
{
	return m_ZoomSpeed;
}

void gui::DCSettingsMenu::SetZoomSpeed(float value)
{
	if (m_ZoomSpeed != value)
	{
		m_ZoomSpeed = value;
		OnPropertyChanged("ZoomSpeed");
	}
}

Noesis::ObservableCollection<gui::VMMonitor>* gui::DCSettingsMenu::GetMonitors() const
{
	return m_Monitors;
}

void gui::DCSettingsMenu::SetMonitorSelected(gui::VMMonitor* value)
{
	if (m_MonitorSelected != value)
	{
		m_MonitorSelected = value;
		OnPropertyChanged("MonitorSelected");
	}
}

gui::VMMonitor* gui::DCSettingsMenu::GetMonitorSelected() const
{
	return m_MonitorSelected;
}

void gui::DCSettingsMenu::OnCloseCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::settings_menu::CloseRequest>();
}

NS_IMPLEMENT_REFLECTION(gui::DCSettingsMenu)
{
	NsProp("EffectVolume", &gui::DCSettingsMenu::GetEffectVolume, &gui::DCSettingsMenu::SetEffectVolume);
	NsProp("MasterVolume", &gui::DCSettingsMenu::GetMasterVolume, &gui::DCSettingsMenu::SetMasterVolume);
	NsProp("MusicVolume", &gui::DCSettingsMenu::GetMusicVolume, &gui::DCSettingsMenu::SetMusicVolume);

	NsProp("MoveSpeed", &gui::DCSettingsMenu::GetMoveSpeed, &gui::DCSettingsMenu::SetMoveSpeed);
	NsProp("ZoomRate", &gui::DCSettingsMenu::GetZoomRate, &gui::DCSettingsMenu::SetZoomRate);
	NsProp("ZoomSpeed", &gui::DCSettingsMenu::GetZoomSpeed, &gui::DCSettingsMenu::SetZoomSpeed);

	NsProp("Monitors", &gui::DCSettingsMenu::GetMonitors);
	NsProp("MonitorSelected", &gui::DCSettingsMenu::GetMonitorSelected, &gui::DCSettingsMenu::SetMonitorSelected);

	NsProp("CloseCommand", &gui::DCSettingsMenu::GetCloseCommand);
}