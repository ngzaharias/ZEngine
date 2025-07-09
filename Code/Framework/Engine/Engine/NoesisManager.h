#pragma once

#include "Core/Map.h"
#include "Core/Set.h"
#include "Input/Key.h"

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

		void ProcessInput(
			const Vector2f& mousePos, 
			const Vector2f& mouseDelta, 
			const Vector2f& scrollDelta, 
			Set<input::EKey>& inout_Held, 
			Set<input::EKey>& inout_Pressed, 
			Set<input::EKey>& inout_Released);

	private:
		const eng::Window* m_Window = nullptr;

		Map<str::Guid, Noesis::Ptr<Noesis::IView>> m_Views = {};
	};
}