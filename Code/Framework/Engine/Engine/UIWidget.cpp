#include "EnginePCH.h"
#include "Engine/UIWidget.h"

#include <NsCore/ReflectionImplement.h>

Noesis::PropertyChangedEventHandler& eng::UIWidget::PropertyChanged()
{
	return m_PropertyChanged;
}

void eng::UIWidget::OnPropertyChanged(const char* name)
{
	if (m_PropertyChanged)
	{
		m_PropertyChanged(this, Noesis::PropertyChangedEventArgs(Noesis::Symbol(name)));
	}
}

NS_IMPLEMENT_REFLECTION(eng::UIWidget)
{
	NsImpl<Noesis::INotifyPropertyChanged>();
}
