#include "GameUIPCH.h"
#include "GameUI/VMMonitor.h"

#include <NsCore/ReflectionImplement.h>

namespace
{
	str::String MonitorToString(const int32 index)
	{
		switch (index)
		{
		case 0: return "Primary";
		case 1: return "Secondary";
		}
		return std::format("Monitor {}", index + 1);
	}
}

gui::VMMonitor::VMMonitor(const int32 index)
	: m_Index(index)
{
	m_Name = MonitorToString(m_Index);
}

const char* gui::VMMonitor::GetName() const
{
	return m_Name.c_str();
}

NS_IMPLEMENT_REFLECTION(gui::VMMonitor)
{
	NsProp("Name", &gui::VMMonitor::GetName);
}