#pragma once

#include "Core/Input.h"
#include "Core/Set.h"
#include "Core/StringView.h"
#include "Engine/WindowConfig.h"
#include "Math/Vector.h"

class GameTime;

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

	private:
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

	protected:
		eng::WindowConfig m_Config = {};
	};
}