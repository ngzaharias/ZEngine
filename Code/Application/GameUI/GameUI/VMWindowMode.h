#pragma once

#include "Engine/WindowModeEnum.h"
#include "Engine/UIDataContext.h"

namespace gui
{
	class VMWindowMode final : public Noesis::BaseComponent
	{
	public:
		VMWindowMode(const eng::EWindowMode windowMode);

		const char* GetName() const;

	public:
		Noesis::String m_Name = {};
		eng::EWindowMode m_WindowMode = eng::EWindowMode::Borderless;

		NS_DECLARE_REFLECTION(gui::VMWindowMode, Noesis::BaseComponent)
	};
}