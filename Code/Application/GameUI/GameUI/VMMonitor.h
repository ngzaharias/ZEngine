#pragma once

#include "Engine/UIDataContext.h"

namespace gui
{
	class VMMonitor final : public Noesis::BaseComponent
	{
	public:
		VMMonitor(const int32 index);

		const char* GetName() const;

	public:
		Noesis::String m_Name = {};
		int32 m_Index = -1;

		NS_DECLARE_REFLECTION(gui::VMMonitor, Noesis::BaseComponent)
	};
}