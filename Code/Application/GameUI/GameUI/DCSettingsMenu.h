#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/UIDataContext.h"

#include <NsApp/DelegateCommand.h>
#include <NsGui/BaseCommand.h>

namespace eng
{
	class WindowManager;
}

namespace eng::settings
{
	struct LocalComponent;
}

namespace gui
{
	class VMMonitor;
	class VMRefreshRate;
	class VMResolution;
	class VMWindowMode;
}

namespace Noesis
{
	template<class T>
	class ObservableCollection;
}

namespace gui
{
	class DCSettingsMenu final : public eng::UIDataContext
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::WindowManager,
			// Components
			const eng::settings::LocalComponent
		>;

		DCSettingsMenu();
		~DCSettingsMenu() override;

		void Initialise(World& world);

		int32 GetEffectVolume() const;
		void SetEffectVolume(int32 value);
		int32 GetMasterVolume() const;
		void SetMasterVolume(int32 value);
		int32 GetMusicVolume() const;
		void SetMusicVolume(int32 value);

		float GetMoveSpeed() const;
		void SetMoveSpeed(float value);
		float GetZoomRate() const;
		void SetZoomRate(float value);
		float GetZoomSpeed() const;
		void SetZoomSpeed(float value);

		Noesis::ObservableCollection<gui::VMMonitor>* GetMonitors() const;
		void SetMonitors(Noesis::Ptr<Noesis::ObservableCollection<gui::VMMonitor>> value);
		gui::VMMonitor* GetMonitor() const;
		void SetMonitor(gui::VMMonitor* value);

		Noesis::ObservableCollection<gui::VMRefreshRate>* GetRefreshRates() const;
		void SetRefreshRates(Noesis::Ptr<Noesis::ObservableCollection<gui::VMRefreshRate>> value);
		gui::VMRefreshRate* GetRefreshRate() const;
		void SetRefreshRate(gui::VMRefreshRate* value);

		Noesis::ObservableCollection<gui::VMResolution>* GetResolutions() const;
		void SetResolutions(Noesis::Ptr<Noesis::ObservableCollection<gui::VMResolution>> value);
		gui::VMResolution* GetResolution() const;
		void SetResolution(gui::VMResolution* value);

		Noesis::ObservableCollection<gui::VMWindowMode>* GetWindowModes() const;
		void SetWindowModes(Noesis::Ptr<Noesis::ObservableCollection<gui::VMWindowMode>> value);
		gui::VMWindowMode* GetWindowMode() const;
		void SetWindowMode(gui::VMWindowMode* value);

		const NoesisApp::DelegateCommand* GetCloseCommand() const { return &m_CloseCommand; }

	private:
		void OnCloseCommand(Noesis::BaseComponent* param);

	private:
		NoesisApp::DelegateCommand m_CloseCommand = {};

	private:
		// audio
		int32 m_EffectVolume = 0;
		int32 m_MasterVolume = 0;
		int32 m_MusicVolume = 0;
		// gameplay
		float m_MoveSpeed = 0.f;
		float m_ZoomRate = 0.f;
		float m_ZoomSpeed = 0.f;
		// window
		Noesis::Ptr<Noesis::ObservableCollection<gui::VMMonitor>> m_Monitors;
		Noesis::Ptr<Noesis::ObservableCollection<gui::VMRefreshRate>> m_RefreshRates;
		Noesis::Ptr<Noesis::ObservableCollection<gui::VMResolution>> m_Resolutions;
		Noesis::Ptr<Noesis::ObservableCollection<gui::VMWindowMode>> m_WindowModes;
		gui::VMMonitor* m_Monitor = nullptr;
		gui::VMRefreshRate* m_RefreshRate = nullptr;
		gui::VMResolution* m_Resolution = nullptr;
		gui::VMWindowMode* m_WindowMode = nullptr;

		NS_DECLARE_REFLECTION(gui::DCSettingsMenu, eng::UIDataContext)
	};
}