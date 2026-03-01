#pragma once

#include "Core/String.h"
#include "Engine/UIViewModel.h"

namespace gui
{
	class VMRefreshRate final : public eng::UIViewModel
	{
	public:
		VMRefreshRate(const int32 refreshRate, bool isNative);

		const char* GetName() const;

	public:
		str::String m_Name = {};
		int32 m_RefreshRate = -1;

	private:
		NS_DECLARE_REFLECTION(gui::VMRefreshRate, Noesis::BaseComponent)
	};
}