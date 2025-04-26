#pragma once

#include "Core/Array.h"
#include "Engine/Monitor.h"
#include "Engine/WindowModeEnum.h"

class GameTime;

namespace eng
{
	class Window;
	struct WindowConfig;
}

namespace eng
{
	class WindowManager final
	{
	public:
		void Initialise();
		void Shutdown();

		void PreUpdate(const GameTime& gameTime);
		void PostUpdate(const GameTime& gameTime);

		auto Create(const eng::WindowConfig& config) -> const eng::Window*;
		bool Destroy(const eng::Window* value);

		auto GetMonitor(const int32 index) const -> const eng::Monitor*;
		auto GetMonitors() const -> const Array<eng::Monitor>&;

		auto GetWindow(const int32 index) -> eng::Window*;
		auto GetWindow(const int32 index) const -> const eng::Window*;
		auto GetWindows() const -> const Array<eng::Window*>&;

		const Array<eng::EWindowMode>& GetWindowModes() const { return m_WindowModes; }
		const Array<Vector2u>& GetResolutions() const { return m_Resolutions; }
		const Array<int32>& GetRefreshRates() const { return m_RefreshRates; }

	private:
		Array<eng::Monitor> m_Monitors = {};
		Array<eng::Window*> m_Windows = {};

		Array<eng::EWindowMode> m_WindowModes = {};
		Array<Vector2u> m_Resolutions = {};
		Array<int32> m_RefreshRates = {};
	};
}
