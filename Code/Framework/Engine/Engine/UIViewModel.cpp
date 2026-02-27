#include "EnginePCH.h"
#include "Engine/UIViewModel.h"

#include <NsCore/ReflectionImplement.h>

eng::UIViewModel::~UIViewModel()
{
}

Noesis::PropertyChangedEventHandler& eng::UIViewModel::PropertyChanged()
{
	return m_PropertyChanged;
}

void eng::UIViewModel::OnPropertyChanged(const char* name)
{
	if (m_PropertyChanged)
	{
		m_PropertyChanged(this, Noesis::PropertyChangedEventArgs(Noesis::Symbol(name)));
	}
}

NS_IMPLEMENT_REFLECTION(eng::UIViewModel)
{
	NsImpl<Noesis::INotifyPropertyChanged>();
}
