#include "EnginePCH.h"
#include "Engine/UIDataContext.h"

#include <NsCore/ReflectionImplement.h>

eng::UIDataContext::~UIDataContext()
{
}

Noesis::PropertyChangedEventHandler& eng::UIDataContext::PropertyChanged()
{
	return m_PropertyChanged;
}

void eng::UIDataContext::OnPropertyChanged(const char* name)
{
	if (m_PropertyChanged)
	{
		m_PropertyChanged(this, Noesis::PropertyChangedEventArgs(Noesis::Symbol(name)));
	}
}

NS_IMPLEMENT_REFLECTION(eng::UIDataContext)
{
	NsImpl<Noesis::INotifyPropertyChanged>();
}
