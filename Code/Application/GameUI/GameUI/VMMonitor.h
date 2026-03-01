#pragma once

#include "Core/String.h"
#include "Engine/UIViewModel.h"

namespace gui
{
	class VMMonitor final : public eng::UIViewModel
	{
	public:
		VMMonitor(const int32 index);

		const char* GetName() const;

	public:
		str::String m_Name = {};
		int32 m_Index = -1;

	private:
		NS_DECLARE_REFLECTION(gui::VMMonitor, Noesis::BaseComponent)
	};
}