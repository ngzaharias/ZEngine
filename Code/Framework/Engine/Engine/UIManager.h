#pragma once

#include "Core/Array.h"
#include "Core/Map.h"
#include "Core/Name.h"

#include <NsCore/BaseComponent.h>
#include <NsCore/Delegate.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Ptr.h>
#include <NsGui/INotifyPropertyChanged.h>
#include <NsGui/IView.h>
#include <NsGui/ResourceDictionary.h>

class GameTime;
template<typename Type>
class Set;
class Vector2f;

namespace ecs
{
	class EntityWorld;
}

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
	struct Widget
	{
		Noesis::Ptr<Noesis::IView> m_View = {};
		bool m_CosumeAll = false;
	};

	class UIManager final
	{
	public:
		UIManager(ecs::EntityWorld& entityWorld);

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

		//////////////////////////////////////////////////////////////////////////
		// Widgets
		void CreateWidget(const str::Name& name, const bool consumeAll = false);
		void DestroyWidget(const str::Name& name);

		template<typename TDataContext>
		void RegisterDataContext(const str::Name& name);

		template<typename TDataContext>
		auto ReadDataContext(const str::Name& name)->const TDataContext&;

		template<typename TDataContext>
		auto WriteDataContext(const str::Name& name)->TDataContext&;

		void SetResource(const char* key, Noesis::Ptr<Noesis::BaseComponent> value);

	private:
		ecs::EntityWorld& m_EntityWorld;
		const eng::Window* m_Window = nullptr;
		Noesis::Ptr<Noesis::ResourceDictionary> m_ResourceDictionary = {};

		Array<str::Name> m_Stack = {};
		Map<str::Name, Noesis::Ptr<Noesis::BaseComponent>> m_DataContexts = {};
		Map<str::Name, eng::Widget> m_Widgets = {};
	};
}

#include "UIManager.inl"