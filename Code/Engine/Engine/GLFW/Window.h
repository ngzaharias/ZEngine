#pragma once

#ifdef Z_GLFW
#include "Core/Input.h"
#include "Core/Set.h"
#include "Engine/Window.h"
#include "Math/Vector.h"

struct GLFWwindow;

namespace input
{
	enum class EKeyboard;
	enum class EMouse;
}

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

		bool HasResized() const override { return m_HasResized; }
		bool ShouldClose() const override;

		void GatherKeyboard(Set<input::EKeyboard>& out_Keys) const override;
		void GatherMouse(Set<input::EMouse>& out_Keys, Vector2f& out_Delta, Vector2f& out_Position) const override;
		void GatherScroll(Vector2f& out_Delta) const override;

		void Refresh() override;

	private:
		static void Callback_FramebufferResized(GLFWwindow* glfwWindow, int width, int height);
		static void Callback_ScrollChanged(GLFWwindow* glfwWindow, double xOffset, double yOffset);

	private:
		Vector2f m_MousePos = Vector2f::Zero;
		Vector2f m_MouseDelta = Vector2f::Zero;
		Vector2f m_ScrollDelta = Vector2f::Zero;

		GLFWwindow* m_Window = nullptr;
		bool m_HasResized = false;
	};
}

#endif