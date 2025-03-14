#pragma once

#include "Core/Input.h"
#include "Core/Set.h"
#include "Core/StringView.h"
#include "Engine/WindowConfig.h"
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

		virtual bool HasResized() const { return false; }
		virtual bool ShouldClose() const { return false; }

		virtual void GatherKeyboard(Set<input::EKeyboard>& out_Keys) const {}
		virtual void GatherMouse(Set<input::EMouse>& out_Keys, Vector2f& out_Delta, Vector2f& out_Position) const {}
		virtual void GatherScroll(Vector2f& out_Delta) const {}

		virtual void SetMode(const eng::EWindowMode value) 
		{ 
			m_IsDirty = value != m_Config.m_Mode;
			m_Config.m_Mode = value; 
		}

		virtual void SetResolution(const Vector2u& value) 
		{ 
			m_IsDirty = value != m_Config.m_Resolution;
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