#pragma once

#include <NsCore/BaseComponent.h>
#include <NsCore/Delegate.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/INotifyPropertyChanged.h>

namespace ecs
{
	class EntityWorld;
}

namespace eng
{
	class UIDataContext : public Noesis::BaseComponent, public Noesis::INotifyPropertyChanged
	{
		friend class UIManager;

	public:
		virtual ~UIDataContext();

		Noesis::PropertyChangedEventHandler& PropertyChanged() override final;

		NS_IMPLEMENT_INTERFACE_FIXUP

	protected:
		void OnPropertyChanged(const char* name);

	protected:
		ecs::EntityWorld* m_EntityWorld = nullptr;

	private:
		Noesis::PropertyChangedEventHandler m_PropertyChanged;

		NS_DECLARE_REFLECTION(eng::UIDataContext, Noesis::BaseComponent)
	};
}
