#pragma once

#include "Engine/UIDataContext.h"

namespace gui
{
	class VMRefreshRate final : public Noesis::BaseComponent
	{
	public:
		VMRefreshRate(const int32 refreshRate, bool isNative);

		const char* GetName() const;

	public:
		Noesis::String m_Name = {};
		int32 m_RefreshRate = -1;

		NS_DECLARE_REFLECTION(gui::VMRefreshRate, Noesis::BaseComponent)
	};
}