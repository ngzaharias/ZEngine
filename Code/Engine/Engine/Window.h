#pragma once

#include "Core/StringView.h"
#include "Math/Vector.h"

class GameTime;

namespace eng
{
	struct WindowConfig
	{
		str::String m_Name = "ZEngine";
		Vector2u m_Position = Vector2u::Zero; 
		Vector2u m_Size = Vector2u(1920, 1080);
		bool m_IsFullscreen = false;
		bool m_IsVSyncEnabled = false;
	};

	class Window
	{
	public:
		explicit Window(const WindowConfig& config) 
			: m_Config(config) { }
		virtual ~Window() { }

		virtual void Initialize() { }
		virtual void Shutdown() { }

		virtual void PreUpdate(const GameTime& gameTime) { }
		virtual void PostUpdate(const GameTime& gameTime) { }

		virtual Vector2u GetSize() const = 0;

		virtual bool HasResized() const { return false; }
		virtual bool ShouldClose() const { return false; }

	private:
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

	protected:
		eng::WindowConfig m_Config = {};
	};
}