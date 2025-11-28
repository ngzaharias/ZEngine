#include "GameUIPCH.h"
#include "GameUI/DCSettingsMenu.h"

#include "Core/Algorithms.h"
#include "Core/EnumHelpers.h"
#include "ECS/EntityWorld.h"
#include "Engine/AudioSettingsComponent.h"
#include "Engine/CameraSettingsComponent.h"
#include "Engine/GameplaySettingsComponent.h"
#include "Engine/SettingsWindowSingleton.h"
#include "Engine/ThemeTable.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "GameUI/SettingsMenuComponents.h"
#include "GameUI/VMMonitor.h"
#include "GameUI/VMRefreshRate.h"
#include "GameUI/VMResolution.h"
#include "GameUI/VMTheme.h"
#include "GameUI/VMWindowMode.h"

#include <NsCore/ReflectionImplement.h>
#include <NsGui/ObservableCollection.h>

gui::DCSettingsMenu::DCSettingsMenu()
{
	m_Themes = Noesis::MakePtr<Noesis::ObservableCollection<gui::VMTheme>>();
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
	const auto& audio = world.ReadSingleton<eng::settings::AudioSingleton>();
	const auto& camera = world.ReadSingleton<eng::settings::CameraSingleton>();
	const auto& gameplay = world.ReadSingleton<eng::settings::GameplaySingleton>();
	const auto& themes = world.ReadResource<eng::ThemeTable>();
	const auto& window = world.ReadSingleton<eng::settings::WindowSingleton>();
	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const auto& monitor = *windowManager.GetMonitor(0);

	// audio
	m_EffectVolume = audio.m_EffectVolume;
	m_MasterVolume = audio.m_MasterVolume;
	m_MusicVolume = audio.m_MusicVolume;

	// gameplay
	m_MoveSpeed = camera.m_TranslateSpeed;
	m_ZoomRate = camera.m_ZoomAmount;
	m_ZoomSpeed = camera.m_ZoomSpeed;

	m_Themes->Clear();
	for (const auto& [guid, value] : themes.GetObjectMap())
	{
		auto theme = Noesis::MakePtr<gui::VMTheme>(value.m_Name, guid);
		m_Themes->Add(theme);
		if (guid == gameplay.m_Theme)
			m_Theme = theme;
	}

	// window
	m_Monitors->Clear();
	for (const auto& [i, value] : enumerate::Forward(windowManager.GetMonitors()))
	{
		auto monitor = Noesis::MakePtr<gui::VMMonitor>(i);
		m_Monitors->Add(monitor);
		if (i == window.m_Monitor)
			m_Monitor = monitor;
	}

	m_RefreshRates->Clear();
	for (const int32 value : windowManager.GetRefreshRates())
	{
		const bool isNative = value == monitor.m_RefreshRate;
		auto refreshRate = Noesis::MakePtr<gui::VMRefreshRate>(value, isNative);
		m_RefreshRates->Add(refreshRate);
		if (value == window.m_RefreshRate)
			m_RefreshRate = refreshRate;
	}

	m_Resolutions->Clear();
	for (const Vector2u& value : windowManager.GetResolutions())
	{
		const bool isNative = value == monitor.m_Resolution;
		auto resolution = Noesis::MakePtr<gui::VMResolution>(value, isNative);
		m_Resolutions->Add(resolution);
		if (value == window.m_Resolution)
			m_Resolution = resolution;
	}

	m_WindowModes->Clear();
	for (const eng::EWindowMode value : windowManager.GetWindowModes())
	{
		auto windowMode = Noesis::MakePtr<gui::VMWindowMode>(value);
		m_WindowModes->Add(windowMode);
		if (value == window.m_WindowMode)
			m_WindowMode = windowMode;
	}

	OnPropertyChanged("");
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

		auto& eventData = m_EntityWorld->AddEvent<gui::settings_menu::ValueRequest>();
		eventData.m_EffectVolume = value;
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

		auto& eventData = m_EntityWorld->AddEvent<gui::settings_menu::ValueRequest>();
		eventData.m_MasterVolume = value;
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

		auto& eventData = m_EntityWorld->AddEvent<gui::settings_menu::ValueRequest>();
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

		auto& eventData = m_EntityWorld->AddEvent<gui::settings_menu::ValueRequest>();
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

		auto& eventData = m_EntityWorld->AddEvent<gui::settings_menu::ValueRequest>();
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

		auto& eventData = m_EntityWorld->AddEvent<gui::settings_menu::ValueRequest>();
		eventData.m_ZoomSpeed = value;
	}
}

Noesis::ObservableCollection<gui::VMTheme>* gui::DCSettingsMenu::GetThemes() const
{
	return m_Themes;
}

void gui::DCSettingsMenu::SetThemes(Noesis::Ptr<Noesis::ObservableCollection<gui::VMTheme>> value)
{
	if (m_Themes != value)
	{
		m_Themes = value;
		OnPropertyChanged("Themes");
	}
}

gui::VMTheme* gui::DCSettingsMenu::GetTheme() const
{
	return m_Theme;
}

void gui::DCSettingsMenu::SetTheme(gui::VMTheme* value)
{
	if (m_Theme != value)
	{
		m_Theme = value;
		OnPropertyChanged("Theme");

		if (value)
		{
			auto& eventData = m_EntityWorld->AddEvent<gui::settings_menu::ValueRequest>();
			eventData.m_Theme = value->m_Guid;
		}
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
			auto& eventData = m_EntityWorld->AddEvent<gui::settings_menu::ValueRequest>();
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
			auto& eventData = m_EntityWorld->AddEvent<gui::settings_menu::ValueRequest>();
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
			auto& eventData = m_EntityWorld->AddEvent<gui::settings_menu::ValueRequest>();
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
			auto& eventData = m_EntityWorld->AddEvent<gui::settings_menu::ValueRequest>();
			eventData.m_WindowMode = value->m_WindowMode;
		}
	}
}

void gui::DCSettingsMenu::OnCloseCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEvent<gui::settings_menu::CloseRequest>();
}

NS_IMPLEMENT_REFLECTION(gui::DCSettingsMenu)
{
	// audio
	NsProp("EffectVolume", &gui::DCSettingsMenu::GetEffectVolume, &gui::DCSettingsMenu::SetEffectVolume);
	NsProp("MasterVolume", &gui::DCSettingsMenu::GetMasterVolume, &gui::DCSettingsMenu::SetMasterVolume);
	NsProp("MusicVolume", &gui::DCSettingsMenu::GetMusicVolume, &gui::DCSettingsMenu::SetMusicVolume);

	// camera
	NsProp("MoveSpeed", &gui::DCSettingsMenu::GetMoveSpeed, &gui::DCSettingsMenu::SetMoveSpeed);
	NsProp("ZoomRate", &gui::DCSettingsMenu::GetZoomRate, &gui::DCSettingsMenu::SetZoomRate);
	NsProp("ZoomSpeed", &gui::DCSettingsMenu::GetZoomSpeed, &gui::DCSettingsMenu::SetZoomSpeed);

	// gameplay
	NsProp("Themes", &gui::DCSettingsMenu::GetThemes);
	NsProp("Theme", &gui::DCSettingsMenu::GetTheme, &gui::DCSettingsMenu::SetTheme);

	// window
	NsProp("Monitors", &gui::DCSettingsMenu::GetMonitors);
	NsProp("Monitor", &gui::DCSettingsMenu::GetMonitor, &gui::DCSettingsMenu::SetMonitor);
	NsProp("RefreshRates", &gui::DCSettingsMenu::GetRefreshRates);
	NsProp("RefreshRate", &gui::DCSettingsMenu::GetRefreshRate, &gui::DCSettingsMenu::SetRefreshRate);
	NsProp("Resolutions", &gui::DCSettingsMenu::GetResolutions);
	NsProp("Resolution", &gui::DCSettingsMenu::GetResolution, &gui::DCSettingsMenu::SetResolution);
	NsProp("WindowModes", &gui::DCSettingsMenu::GetWindowModes);
	NsProp("WindowMode", &gui::DCSettingsMenu::GetWindowMode, &gui::DCSettingsMenu::SetWindowMode);

	// commands
	NsProp("CloseCommand", &gui::DCSettingsMenu::GetCloseCommand);
}