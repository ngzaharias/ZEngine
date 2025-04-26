#pragma once

#include "Core/Set.h"
#include "Math/Vector.h"

class GameTime;

namespace eng
{
	enum class EWindowMode;
	struct WindowConfig;
}

namespace input
{
	enum class EKey;
}

namespace eng
{
	class Window
	{
	public:
		explicit Window(const WindowConfig& config) { }
		virtual ~Window() { }

		virtual void PreUpdate(const GameTime& gameTime) { }
		virtual void PostUpdate(const GameTime& gameTime) { }

		virtual bool ShouldClose() const { return false; }

		virtual void GatherGamepad(Set<input::EKey>& out_Keys) const {}
		virtual void GatherKeyboard(Set<input::EKey>& out_Keys) const {}
		virtual void GatherMouse(Set<input::EKey>& out_Keys, Vector2f& out_Delta, Vector2f& out_Position) const {}
		virtual void GatherScroll(Vector2f& out_Delta) const {}

		virtual auto GetResolution() const -> const Vector2u& { return m_Resolution; }
		virtual auto GetPosition() const -> const Vector2i& { return m_Position; }
		virtual auto GetRefreshRate() const -> int32 { return m_RefreshRate; }

		virtual void Refresh(const eng::EWindowMode& windowMode, const Vector2u& resolution, const int32 refreshRate, const int32 monitor) {}

	private:
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

	protected:
		Vector2u m_Resolution = Vector2u::Zero;
		Vector2i m_Position = Vector2i::Zero;
		Vector2f m_Scale = Vector2f::Zero;
		int32 m_RefreshRate = 0;

		bool m_IsFocused = false;
		bool m_IsIconified = false;
		bool m_IsMaximized = false;
	};
}