#pragma once

#include "Engine/UIDataContext.h"

#include <NsApp/DelegateCommand.h>
#include <NsGui/BaseCommand.h>

namespace Noesis
{
	template<class T>
	class ObservableCollection;
}

namespace gui
{
	class VMMonitor final : public Noesis::BaseComponent
	{
	public:
		VMMonitor(const char* name);

		const char* GetName() const;

	private:
		Noesis::String m_Name;

		NS_DECLARE_REFLECTION(gui::VMMonitor, Noesis::BaseComponent)
	};

	class VMMode final : public Noesis::BaseComponent
	{
	public:
		VMMode(const char* name);

		const char* GetName() const;

	private:
		Noesis::String m_Name;

		NS_DECLARE_REFLECTION(gui::VMMode, Noesis::BaseComponent)
	};

	class VMResolution final : public Noesis::BaseComponent
	{
	public:
		VMResolution(const char* name);

		const char* GetName() const;

	private:
		Noesis::String m_Name;

		NS_DECLARE_REFLECTION(gui::VMResolution, Noesis::BaseComponent)
	};

	class VMRefreshRate final : public Noesis::BaseComponent
	{
	public:
		VMRefreshRate(const char* name);

		const char* GetName() const;

	private:
		Noesis::String m_Name;

		NS_DECLARE_REFLECTION(gui::VMRefreshRate, Noesis::BaseComponent)
	};

	class DCSettingsMenu final : public eng::UIDataContext
	{
	public:
		DCSettingsMenu();

		float GetEffectVolume() const;
		void SetEffectVolume(float value);
		float GetMasterVolume() const;
		void SetMasterVolume(float value);
		float GetMusicVolume() const;
		void SetMusicVolume(float value);

		float GetMoveSpeed() const;
		void SetMoveSpeed(float value);
		float GetZoomRate() const;
		void SetZoomRate(float value);
		float GetZoomSpeed() const;
		void SetZoomSpeed(float value);

		Noesis::ObservableCollection<gui::VMMode>* GetModes() const;
		gui::VMMode* GetMode() const;
		void SetMode(gui::VMMode* value);

		Noesis::ObservableCollection<gui::VMMonitor>* GetMonitors() const;
		gui::VMMonitor* GetMonitor() const;
		void SetMonitor(gui::VMMonitor* value);

		Noesis::ObservableCollection<gui::VMRefreshRate>* GetRefreshRates() const;
		gui::VMRefreshRate* GetRefreshRate() const;
		void SetRefreshRate(gui::VMRefreshRate* value);

		Noesis::ObservableCollection<gui::VMResolution>* GetResolutions() const;
		gui::VMResolution* GetResolution() const;
		void SetResolution(gui::VMResolution* value);

		const NoesisApp::DelegateCommand* GetCloseCommand() const { return &m_CloseCommand; }

	private:
		void OnCloseCommand(Noesis::BaseComponent* param);

	private:
		NoesisApp::DelegateCommand m_CloseCommand = {};

	private:
		// audio
		float m_EffectVolume = 0.f;
		float m_MasterVolume = 0.f;
		float m_MusicVolume = 0.f;
		// gameplay
		float m_MoveSpeed = 0.f;
		float m_ZoomRate = 0.f;
		float m_ZoomSpeed = 0.f;
		// window
		Noesis::Ptr<Noesis::ObservableCollection<gui::VMMode>> m_Modes;
		Noesis::Ptr<Noesis::ObservableCollection<gui::VMMonitor>> m_Monitors;
		Noesis::Ptr<Noesis::ObservableCollection<gui::VMRefreshRate>> m_RefreshRates;
		Noesis::Ptr<Noesis::ObservableCollection<gui::VMResolution>> m_Resolutions;
		gui::VMMode* m_Mode = nullptr;
		gui::VMMonitor* m_Monitor = nullptr;
		gui::VMRefreshRate* m_RefreshRate = nullptr;
		gui::VMResolution* m_Resolution = nullptr;

		NS_DECLARE_REFLECTION(gui::DCSettingsMenu, eng::UIDataContext)
	};
}