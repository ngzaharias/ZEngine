#pragma once

#include <NsCore/Ptr.h>
#include <NsGui/IView.h>

class GameTime;

namespace eng
{
	class Window;
}

namespace ui
{
	class NoesisManager final
	{
	public:
		void Initialise(const eng::Window& window);
		void Shutdown();

		void Update(const GameTime& gameTime);

		void RenderBegin();
		void RenderFinish();

	private:
		const eng::Window* m_Window = nullptr;

		Noesis::Ptr<Noesis::IView> m_View = {};
	};
}