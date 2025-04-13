#pragma once

#include "Core/Array.h"
#include "Engine/WindowConfig.h"

class GameTime;

namespace eng
{
	class Window;
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

		auto GetWindow(const int32 index) -> eng::Window*;
		auto GetWindow(const int32 index) const -> const eng::Window*;

		const Array<int32>& GetRefreshRates() const { return m_RefreshRates; }
		const Array<Vector2u>& GetResolutions() const { return m_Resolutions; }
		const Array<EWindowMode>& GetModes() const { return m_Modes; }

	private:
		Array<int32> m_RefreshRates = {};
		Array<Vector2u> m_Resolutions = {};
		Array<eng::EWindowMode> m_Modes = {};
		Array<eng::Window*> m_Windows = {};
	};
}
