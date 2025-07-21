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
	const str::String string = MonitorToString(m_Index);
	m_Name = Noesis::String(string.data(), (uint32)string.size());
}

const char* gui::VMMonitor::GetName() const
{
	return m_Name.Str();
}

NS_IMPLEMENT_REFLECTION(gui::VMMonitor)
{
	NsProp("Name", &gui::VMMonitor::GetName);
}