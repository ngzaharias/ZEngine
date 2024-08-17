#pragma once

#ifdef Z_GLFW
#include "Core/Set.h"
#include "Core/String.h"
#include "Engine/IWindow.h"
#include "Math/Vector.h"

struct GLFWwindow;

namespace input
{
	enum class EKeyboard;
	enum class EMouse;
}

namespace glfw
{
	class Window final : public eng::IWindow
	{
	public:
		explicit Window(const eng::WindowConfig& config);
		~Window() override;

		void Initialize() override;
		void Shutdown() override;

		void PreUpdate(const GameTime& gameTime) override;
		void PostUpdate(const GameTime& gameTime) override;

		Vector2u GetSize() const override { return m_Size; }
		GLFWwindow* GetWindow() const { return m_Window; }

		bool HasResized() const override { return m_HasResized; }
		void SetResized(bool value) { m_HasResized = value; }

		bool ShouldClose() const override;

		void GatherKeyboard(Set<input::EKeyboard>& out_Keys) const;
		void GatherMouse(Set<input::EMouse>& out_Keys, Vector2f& out_Delta, Vector2f& out_Position) const;

	private:
		static void FramebufferResizeCallback(GLFWwindow* glfwWindow, int width, int height);

	private:
		str::String m_Name = { };
		Vector2u m_Size = { };

		Vector2f m_MousePos = Vector2f::Zero;
		Vector2f m_MouseDelta = Vector2f::Zero;

		GLFWwindow* m_Window = nullptr;
		bool m_HasResized = false;
	};
}

#endif