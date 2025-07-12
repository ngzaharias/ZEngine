#pragma once

template<typename Type>
class Set;
class Vector2f;

namespace eng
{
	class Window;
}

namespace input
{
	enum class EKey;
}

namespace eng
{
	class ImguiManager final
	{
	public:
		void Initialise(const eng::Window& handle);
		void Shutdown();

		/// \brief Called at the start of each frame before any imgui requests are made.
		void PreUpdate();
		/// \brief Called at the end of each frame after all imgui requests are made.
		void PostUpdate();

		void ProcessInput(
			const Vector2f& mousePos,
			const Vector2f& mouseDelta,
			const Vector2f& scrollDelta,
			Set<input::EKey>& inout_Held,
			Set<input::EKey>& inout_Pressed,
			Set<input::EKey>& inout_Released);

	private:
		const eng::Window* m_Window = nullptr;
	};
}