#pragma once

#include <Engine/System.h>

namespace glfw
{
	class Window;
}

namespace editor
{
	class SceneEditor final : public ecs::System
	{
		using Selection = ecs::Entity;

	public:
		SceneEditor(glfw::Window& window);

		void Update(const GameTime& gameTime) override;

		bool IsVisible() { return m_IsVisible; }
		void SetVisible(const bool value) { m_IsVisible = value; }

	private:
		glfw::Window& m_Window;

		bool m_IsVisible = false;
	};
}