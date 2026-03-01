#pragma once

#include "Core/String.h"
#include "Engine/UIViewModel.h"
#include "Engine/WindowModeEnum.h"

namespace gui
{
	class VMWindowMode final : public eng::UIViewModel
	{
	public:
		VMWindowMode(const eng::EWindowMode windowMode);

		const char* GetName() const;

	public:
		str::String m_Name = {};
		eng::EWindowMode m_WindowMode = eng::EWindowMode::Borderless;

	private:
		NS_DECLARE_REFLECTION(gui::VMWindowMode, Noesis::BaseComponent)
	};
}