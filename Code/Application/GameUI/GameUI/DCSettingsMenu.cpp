#include "GameUIPCH.h"
#include "GameUI/DCSettingsMenu.h"

#include "ECS/EntityWorld.h"
#include "GameUI/SettingsMenuComponents.h"

#include <NsCore/ReflectionImplement.h>
#include <NsGui/ObservableCollection.h>

gui::VMMode::VMMode(const char* name) 
	: m_Name(name)
{
}

const char* gui::VMMode::GetName() const
{
	return m_Name.Str();
}

NS_IMPLEMENT_REFLECTION(gui::VMMode)
{
	NsProp("Name", &gui::VMMode::GetName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

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

gui::VMRefreshRate::VMRefreshRate(const char* name)
	: m_Name(name)
{
}

const char* gui::VMRefreshRate::GetName() const
{
	return m_Name.Str();
}

NS_IMPLEMENT_REFLECTION(gui::VMRefreshRate)
{
	NsProp("Name", &gui::VMRefreshRate::GetName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

gui::VMResolution::VMResolution(const char* name)
	: m_Name(name)
{
}

const char* gui::VMResolution::GetName() const
{
	return m_Name.Str();
}

NS_IMPLEMENT_REFLECTION(gui::VMResolution)
{
	NsProp("Name", &gui::VMResolution::GetName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

gui::DCSettingsMenu::DCSettingsMenu()
{
	m_Modes = *new Noesis::ObservableCollection<gui::VMMode>();
	m_Modes->Add(Noesis::MakePtr<gui::VMMode>("Borderless"));
	m_Modes->Add(Noesis::MakePtr<gui::VMMode>("Fullscreen"));
	m_Modes->Add(Noesis::MakePtr<gui::VMMode>("Windowed"));

	m_Monitors = *new Noesis::ObservableCollection<gui::VMMonitor>();
	m_Monitors->Add(Noesis::MakePtr<gui::VMMonitor>("Primary"));
	m_Monitors->Add(Noesis::MakePtr<gui::VMMonitor>("Secondary"));

	m_RefreshRates = *new Noesis::ObservableCollection<gui::VMRefreshRate>();
	m_RefreshRates->Add(Noesis::MakePtr<gui::VMRefreshRate>("59"));
	m_RefreshRates->Add(Noesis::MakePtr<gui::VMRefreshRate>("60"));
	m_RefreshRates->Add(Noesis::MakePtr<gui::VMRefreshRate>("144"));

	m_Resolutions = *new Noesis::ObservableCollection<gui::VMResolution>();
	m_Resolutions->Add(Noesis::MakePtr<gui::VMResolution>("800x600"));
	m_Resolutions->Add(Noesis::MakePtr<gui::VMResolution>("1920x1080"));

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

Noesis::ObservableCollection<gui::VMMode>* gui::DCSettingsMenu::GetModes() const
{
	return m_Modes;
}

gui::VMMode* gui::DCSettingsMenu::GetMode() const
{
	return m_Mode;
}

void gui::DCSettingsMenu::SetMode(gui::VMMode* value)
{
	if (m_Mode != value)
	{
		m_Mode = value;
		OnPropertyChanged("Mode");
	}
}

Noesis::ObservableCollection<gui::VMMonitor>* gui::DCSettingsMenu::GetMonitors() const
{
	return m_Monitors;
}

gui::VMMonitor* gui::DCSettingsMenu::GetMonitor() const
{
	return m_Monitor;
}

void gui::DCSettingsMenu::SetMonitor(gui::VMMonitor* value)
{
	if (m_Monitor != value)
	{
		m_Monitor = value;
		OnPropertyChanged("Monitor");
	}
}

Noesis::ObservableCollection<gui::VMRefreshRate>* gui::DCSettingsMenu::GetRefreshRates() const
{
	return m_RefreshRates;
}

gui::VMRefreshRate* gui::DCSettingsMenu::GetRefreshRate() const
{
	return m_RefreshRate;
}

void gui::DCSettingsMenu::SetRefreshRate(gui::VMRefreshRate* value)
{
	if (m_RefreshRate != value)
	{
		m_RefreshRate = value;
		OnPropertyChanged("RefreshRate");
	}
}

Noesis::ObservableCollection<gui::VMResolution>* gui::DCSettingsMenu::GetResolutions() const
{
	return m_Resolutions;
}

gui::VMResolution* gui::DCSettingsMenu::GetResolution() const
{
	return m_Resolution;
}

void gui::DCSettingsMenu::SetResolution(gui::VMResolution* value)
{
	if (m_Resolution != value)
	{
		m_Resolution = value;
		OnPropertyChanged("Resolution");
	}
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

	NsProp("Modes", &gui::DCSettingsMenu::GetModes);
	NsProp("Mode", &gui::DCSettingsMenu::GetMode, &gui::DCSettingsMenu::SetMode);
	NsProp("Monitors", &gui::DCSettingsMenu::GetMonitors);
	NsProp("Monitor", &gui::DCSettingsMenu::GetMonitor, &gui::DCSettingsMenu::SetMonitor);
	NsProp("RefreshRates", &gui::DCSettingsMenu::GetRefreshRates);
	NsProp("RefreshRate", &gui::DCSettingsMenu::GetRefreshRate, &gui::DCSettingsMenu::SetRefreshRate);
	NsProp("Resolutions", &gui::DCSettingsMenu::GetResolutions);
	NsProp("Resolution", &gui::DCSettingsMenu::GetResolution, &gui::DCSettingsMenu::SetResolution);

	NsProp("CloseCommand", &gui::DCSettingsMenu::GetCloseCommand);
}