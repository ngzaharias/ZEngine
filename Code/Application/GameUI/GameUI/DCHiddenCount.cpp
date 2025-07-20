#include "GameUIPCH.h"
#include "GameUI/DCHiddenCount.h"

#include "ECS/EntityWorld.h"
#include "Hidden/HiddenCountComponent.h"

#include <NsCore/ReflectionImplement.h>

gui::DCHiddenCount::DCHiddenCount()
{
}

int32 gui::DCHiddenCount::GetObjectCount() const
{
	return m_ObjectCount;
}

void gui::DCHiddenCount::SetObjectCount(int32 value)
{
	if (m_ObjectCount != value)
	{
		m_ObjectCount = value;
		OnPropertyChanged("ObjectCount");
	}
}

int32 gui::DCHiddenCount::GetObjectTotal() const
{
	return m_ObjectTotal;
}

void gui::DCHiddenCount::SetObjectTotal(int32 value)
{
	if (m_ObjectTotal != value)
	{
		m_ObjectTotal = value;
		OnPropertyChanged("ObjectTotal");
	}
}

NS_IMPLEMENT_REFLECTION(gui::DCHiddenCount)
{
	NsProp("ObjectCount", &gui::DCHiddenCount::GetObjectCount, &gui::DCHiddenCount::SetObjectCount);
	NsProp("ObjectTotal", &gui::DCHiddenCount::GetObjectTotal, &gui::DCHiddenCount::SetObjectTotal);
}