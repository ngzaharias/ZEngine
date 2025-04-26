#pragma once

#ifdef Z_GLFW
#include "Core/Set.h"
#include "Engine/Window.h"
#include "Math/Vector.h"

struct GLFWwindow;

namespace glfw
{
	class Window final : public eng::Window
	{
	public:
		explicit Window(const eng::WindowConfig& config);
		~Window() override;

		void PreUpdate(const GameTime& gameTime) override;
		void PostUpdate(const GameTime& gameTime) override;

		GLFWwindow* GetWindow() const { return m_Window; }

		bool ShouldClose() const override;

		void GatherGamepad(Set<input::EKey>& out_Keys) const override;
		void GatherKeyboard(Set<input::EKey>& out_Keys) const override;
		void GatherMouse(Set<input::EKey>& out_Keys, Vector2f& out_Delta, Vector2f& out_Position) const override;
		void GatherScroll(Vector2f& out_Delta) const override;

		void Refresh(const eng::EWindowMode& windowMode, const Vector2u& resolution, const int32 refreshRate, const int32 monitor) override;

	private:
		static void Callback_FramebufferResized(GLFWwindow* glfwWindow, int width, int height);
		static void Callback_ScrollChanged(GLFWwindow* glfwWindow, double xOffset, double yOffset);
		static void Callback_WindowClose(GLFWwindow* glfwWindow);
		static void Callback_WindowContentScale(GLFWwindow* glfwWindow, float xscale, float yscale);
		static void Callback_WindowFocus(GLFWwindow* glfwWindow, int focused);
		static void Callback_WindowIconify(GLFWwindow* glfwWindow, int iconified);
		static void Callback_WindowMaximize(GLFWwindow* glfwWindow, int maximized);
		static void Callback_WindowPos(GLFWwindow* glfwWindow, int xpos, int ypos);
		static void Callback_WindowRefresh(GLFWwindow* glfwWindow);
		static void Callback_WindowSize(GLFWwindow* glfwWindow, int width, int height);

	private:
		Vector2f m_MousePos = Vector2f::Zero;
		Vector2f m_MouseDelta = Vector2f::Zero;
		Vector2f m_ScrollDelta = Vector2f::Zero;

		GLFWwindow* m_Window = nullptr;
	};
}

#endif