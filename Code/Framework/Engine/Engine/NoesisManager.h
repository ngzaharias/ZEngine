#pragma once

#include "Core/Map.h"

#include <NsApp/DelegateCommand.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Ptr.h>
#include <NsGui/BaseCommand.h>
#include <NsGui/INotifyPropertyChanged.h>
#include <NsGui/IView.h>

class GameTime;
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

namespace ui
{
	class DCMainMenu final : public Noesis::BaseComponent
	{
	public:
		DCMainMenu();

		Noesis::ICommand* GetNewGameCommand() const;

	private:
		void OnNewGameCommand(Noesis::BaseComponent* param);

	private:
		Noesis::Ptr<NoesisApp::DelegateCommand> m_NewGameCommand;

		NS_DECLARE_REFLECTION(ui::DCMainMenu, Noesis::BaseComponent)
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////

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