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
		void Initialise(const eng::WindowConfig& config);
		void Shutdown();

		void PreUpdate(const GameTime& gameTime);
		void PostUpdate(const GameTime& gameTime);

		auto Create(const eng::WindowConfig& config) -> const eng::Window*;
		bool Destroy(const eng::Window* value);

		const eng::Window* GetPrimary() const { return m_Windows[0]; }
		const eng::Window* GetWindow(const int32 index) const { return m_Windows[index]; }

	private:
		Array<eng::Window*> m_Windows = {};
	};
}
