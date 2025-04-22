#pragma once

#include "Core/Set.h"
#include "Core/StringView.h"
#include "Engine/WindowConfig.h"
#include "Input/Key.h"
#include "Math/Vector.h"

class GameTime;

namespace eng
{
	enum class EWindowMode;
}

namespace eng
{
	class Window
	{
	public:
		explicit Window(const WindowConfig& config) 
			: m_Config(config) { }
		virtual ~Window() { }

		virtual void PreUpdate(const GameTime& gameTime) { }
		virtual void PostUpdate(const GameTime& gameTime) { }

		virtual bool ShouldClose() const { return false; }

		virtual void GatherGamepad(Set<input::EKey>& out_Keys) const {}
		virtual void GatherKeyboard(Set<input::EKey>& out_Keys) const {}
		virtual void GatherMouse(Set<input::EKey>& out_Keys, Vector2f& out_Delta, Vector2f& out_Position) const {}
		virtual void GatherScroll(Vector2f& out_Delta) const {}

		virtual auto GetMode() const -> eng::EWindowMode { return m_Config.m_Mode; }
		virtual void SetMode(const eng::EWindowMode value) 
		{ 
			m_IsDirty |= value != m_Config.m_Mode;
			m_Config.m_Mode = value; 
		}

		virtual auto GetRefreshRate() const -> int32 { return m_Config.m_RefreshRate; }
		virtual void SetRefreshRate(const int32 value)
		{
			m_IsDirty |= value != m_Config.m_RefreshRate;
			m_Config.m_RefreshRate = value;
		}

		virtual auto GetResolution() const -> const Vector2u& { return m_Config.m_Resolution; }
		virtual void SetResolution(const Vector2u& value) 
		{ 
			m_IsDirty |= value != m_Config.m_Resolution;
			m_Config.m_Resolution = value;
		}

		virtual void Refresh() {}

	private:
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

	protected:
		eng::WindowConfig m_Config = {};
		bool m_IsDirty = false;
	};
}