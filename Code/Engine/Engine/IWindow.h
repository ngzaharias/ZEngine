#pragma once

#include "Core/StringView.h"
#include "Core/Vector.h"

class GameTime;

namespace eng
{
	struct WindowConfig
	{
		str::StringView m_Name;
		Vector2u m_Position; 
		Vector2u m_Size; 
	};

	class IWindow
	{
	public:
		explicit IWindow(const WindowConfig& config) { }
		virtual ~IWindow() { }

		virtual void Initialize() { }
		virtual void Shutdown() { }

		virtual void PreUpdate(const GameTime& gameTime) { }
		virtual void PostUpdate(const GameTime& gameTime) { }

		virtual Vector2u GetSize() const = 0;

		virtual bool HasResized() const { return false; }
		virtual bool ShouldClose() const { return false; }

	private:
		IWindow(const IWindow&) = delete;
		IWindow& operator=(const IWindow&) = delete;
	};
}