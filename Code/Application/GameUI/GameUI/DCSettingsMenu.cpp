#include "GameUIPCH.h"
#include "GameUI/DCSettingsMenu.h"

#include "Core/Algorithms.h"
#include "Core/EnumHelpers.h"
#include "ECS/EntityWorld.h"
#include "Engine/SettingsComponents.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "GameUI/SettingsMenuComponents.h"
#include "GameUI/VMMonitor.h"
#include "GameUI/VMRefreshRate.h"
#include "GameUI/VMResolution.h"
#include "GameUI/VMWindowMode.h"

#include <NsCore/ReflectionImplement.h>
#include <NsGui/ObservableCollection.h>

gui::DCSettingsMenu::DCSettingsMenu()
{
	m_Monitors = Noesis::MakePtr<Noesis::ObservableCollection<gui::VMMonitor>>();
	m_RefreshRates = Noesis::MakePtr<Noesis::ObservableCollection<gui::VMRefreshRate>>();
	m_Resolutions = Noesis::MakePtr<Noesis::ObservableCollection<gui::VMResolution>>();
	m_WindowModes = Noesis::MakePtr<Noesis::ObservableCollection<gui::VMWindowMode>>();

	m_CloseCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCSettingsMenu::OnCloseCommand));
}

gui::DCSettingsMenu::~DCSettingsMenu()
{
}

void gui::DCSettingsMenu::Initialise(World& world)
{
	const auto& settings = world.ReadSingleton<eng::settings::LocalComponent>();
	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const auto& monitor = *windowManager.GetMonitor(0);

	// audio
	m_EffectVolume = settings.m_Audio.m_EffectVolume;
	m_MasterVolume = settings.m_Audio.m_MasterVolume;
	m_MusicVolume = settings.m_Audio.m_MusicVolume;

	// gameplay
	m_MoveSpeed = settings.m_Camera.m_TranslateSpeed;
	m_ZoomRate = settings.m_Camera.m_ZoomAmount;
	m_ZoomSpeed = settings.m_Camera.m_ZoomSpeed;

	m_Monitors->Clear();
	for (const auto& [i, value] : enumerate::Forward(windowManager.GetMonitors()))
	{
		auto monitor = Noesis::MakePtr<gui::VMMonitor>(i);
		m_Monitors->Add(monitor);
		if (i == settings.m_Window.m_Monitor)
			m_Monitor = monitor;
	}

	m_RefreshRates->Clear();
	for (const int32 value : windowManager.GetRefreshRates())
	{
		const bool isNative = value == monitor.m_RefreshRate;
		auto refreshRate = Noesis::MakePtr<gui::VMRefreshRate>(value, isNative);
		m_RefreshRates->Add(refreshRate);
		if (value == settings.m_Window.m_RefreshRate)
			m_RefreshRate = refreshRate;
	}

	m_Resolutions->Clear();
	for (const Vector2u& value : windowManager.GetResolutions())
	{
		const bool isNative = value == monitor.m_Resolution;
		auto resolution = Noesis::MakePtr<gui::VMResolution>(value, isNative);
		m_Resolutions->Add(resolution);
		if (value == settings.m_Window.m_Resolution)
			m_Resolution = resolution;
	}

	m_WindowModes->Clear();
	for (const eng::EWindowMode value : windowManager.GetWindowModes())
	{
		auto windowMode = Noesis::MakePtr<gui::VMWindowMode>(value);
		m_WindowModes->Add(windowMode);
		if (value == settings.m_Window.m_WindowMode)
			m_WindowMode = windowMode;
	}

	OnPropertyChanged("");
}

int32 gui::DCSettingsMenu::GetEffectVolume() const
{
	return m_EffectVolume;
}

void gui::DCSettingsMenu::SetEffectVolume(int32 value)
{
	if (m_EffectVolume != value)
	{
		m_EffectVolume = value;
		OnPropertyChanged("EffectVolume");

		auto& eventData = m_EntityWorld->AddEventComponent<gui::settings_menu::ValueRequest>();
		eventData.m_EffectVolume = value;
	}
}

int32 gui::DCSettingsMenu::GetMasterVolume() const
{
	return m_MasterVolume;
}

void gui::DCSettingsMenu::SetMasterVolume(int32 value)
{
	if (m_MasterVolume != value)
	{
		m_MasterVolume = value;
		OnPropertyChanged("MasterVolume");

		auto& eventData = m_EntityWorld->AddEventComponent<gui::settings_menu::ValueRequest>();
		eventData.m_MasterVolume = value;
	}
}

int32 gui::DCSettingsMenu::GetMusicVolume() const
{
	return m_MusicVolume;
}

void gui::DCSettingsMenu::SetMusicVolume(int32 value)
{
	if (m_MusicVolume != value)
	{
		m_MusicVolume = value;
		OnPropertyChanged("MusicVolume");

		auto& eventData = m_EntityWorld->AddEventComponent<gui::settings_menu::ValueRequest>();
		eventData.m_MusicVolume = value;
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

		auto& eventData = m_EntityWorld->AddEventComponent<gui::settings_menu::ValueRequest>();
		eventData.m_MoveSpeed = value;
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

		auto& eventData = m_EntityWorld->AddEventComponent<gui::settings_menu::ValueRequest>();
		eventData.m_ZoomRate = value;
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

		auto& eventData = m_EntityWorld->AddEventComponent<gui::settings_menu::ValueRequest>();
		eventData.m_ZoomSpeed = value;
	}
}

Noesis::ObservableCollection<gui::VMMonitor>* gui::DCSettingsMenu::GetMonitors() const
{
	return m_Monitors;
}

void gui::DCSettingsMenu::SetMonitors(Noesis::Ptr<Noesis::ObservableCollection<gui::VMMonitor>> value)
{
	if (m_Monitors != value)
	{
		m_Monitors = value;
		OnPropertyChanged("Monitors");
	}
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

		if (value)
		{
			auto& eventData = m_EntityWorld->AddEventComponent<gui::settings_menu::ValueRequest>();
			eventData.m_Monitor = value->m_Index;
		}
	}
}

Noesis::ObservableCollection<gui::VMRefreshRate>* gui::DCSettingsMenu::GetRefreshRates() const
{
	return m_RefreshRates;
}

void gui::DCSettingsMenu::SetRefreshRates(Noesis::Ptr<Noesis::ObservableCollection<gui::VMRefreshRate>> value)
{
	if (m_RefreshRates != value)
	{
		m_RefreshRates = value;
		OnPropertyChanged("RefreshRates");
	}
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

		if (value)
		{
			auto& eventData = m_EntityWorld->AddEventComponent<gui::settings_menu::ValueRequest>();
			eventData.m_RefreshRate = value->m_RefreshRate;
		}
	}
}

Noesis::ObservableCollection<gui::VMResolution>* gui::DCSettingsMenu::GetResolutions() const
{
	return m_Resolutions;
}

void gui::DCSettingsMenu::SetResolutions(Noesis::Ptr<Noesis::ObservableCollection<gui::VMResolution>> value)
{
	if (m_Resolutions != value)
	{
		m_Resolutions = value;
		OnPropertyChanged("Resolutions");
	}
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

		if (value)
		{
			auto& eventData = m_EntityWorld->AddEventComponent<gui::settings_menu::ValueRequest>();
			eventData.m_Resolution = value->m_Resolution;
		}
	}
}

Noesis::ObservableCollection<gui::VMWindowMode>* gui::DCSettingsMenu::GetWindowModes() const
{
	return m_WindowModes;
}

void gui::DCSettingsMenu::SetWindowModes(Noesis::Ptr<Noesis::ObservableCollection<gui::VMWindowMode>> value)
{
	if (m_WindowModes != value)
	{
		m_WindowModes = value;
		OnPropertyChanged("WindowModes");
	}
}

gui::VMWindowMode* gui::DCSettingsMenu::GetWindowMode() const
{
	return m_WindowMode;
}

void gui::DCSettingsMenu::SetWindowMode(gui::VMWindowMode* value)
{
	if (m_WindowMode != value)
	{
		m_WindowMode = value;
		OnPropertyChanged("WindowMode");

		if (value)
		{
			auto& eventData = m_EntityWorld->AddEventComponent<gui::settings_menu::ValueRequest>();
			eventData.m_WindowMode = value->m_WindowMode;
		}
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

	NsProp("Monitors", &gui::DCSettingsMenu::GetMonitors);
	NsProp("Monitor", &gui::DCSettingsMenu::GetMonitor, &gui::DCSettingsMenu::SetMonitor);
	NsProp("RefreshRates", &gui::DCSettingsMenu::GetRefreshRates);
	NsProp("RefreshRate", &gui::DCSettingsMenu::GetRefreshRate, &gui::DCSettingsMenu::SetRefreshRate);
	NsProp("Resolutions", &gui::DCSettingsMenu::GetResolutions);
	NsProp("Resolution", &gui::DCSettingsMenu::GetResolution, &gui::DCSettingsMenu::SetResolution);
	NsProp("WindowModes", &gui::DCSettingsMenu::GetWindowModes);
	NsProp("WindowMode", &gui::DCSettingsMenu::GetWindowMode, &gui::DCSettingsMenu::SetWindowMode);

	NsProp("CloseCommand", &gui::DCSettingsMenu::GetCloseCommand);
}