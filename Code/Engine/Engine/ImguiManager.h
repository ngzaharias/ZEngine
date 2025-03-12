#pragma once

namespace eng
{
	class Window;
}

namespace eng
{
	class ImguiManager final
	{
	public:
		void Initialise(const eng::Window* handle);
		void Shutdown();

		/// \brief Called at the start of each frame before any imgui requests are made.
		void PreUpdate();
		/// \brief Called at the end of each frame after all imgui requests are made.
		void PostUpdate();

	private:
		const eng::Window* m_Window;
	};
}