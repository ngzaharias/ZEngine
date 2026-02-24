#pragma once

#include "Core/Name.h"
#include "Engine/UIDataContext.h"

namespace gui
{
	class VMAbility final : public Noesis::BaseComponent
	{
	public:
		VMAbility(const str::Name& name);

		const char* GetName() const;

	public:
		Noesis::String m_Name = {};
		str::Name m_Key = {};

		NS_DECLARE_REFLECTION(gui::VMAbility, Noesis::BaseComponent)
	};
}