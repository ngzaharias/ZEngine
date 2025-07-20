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

		Noesis::ObservableCollection<gui::VMMonitor>* GetMonitors() const;
		void SetMonitorSelected(gui::VMMonitor* value);
		gui::VMMonitor* GetMonitorSelected() const;

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
		Noesis::Ptr<Noesis::ObservableCollection<gui::VMMonitor>> m_Monitors;
		gui::VMMonitor* m_MonitorSelected = nullptr;

		NS_DECLARE_REFLECTION(gui::DCSettingsMenu, eng::UIDataContext)
	};
}