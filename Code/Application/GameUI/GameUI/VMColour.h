#pragma once

#include "Core/Guid.h"
#include "Engine/UIDataContext.h"

namespace str
{
	class Name;
}

namespace gui
{
	class VMColour final : public Noesis::BaseComponent
	{
	public:
		VMColour(const str::Name& name, const str::Guid& guid);

		const char* GetName() const;

	public:
		Noesis::String m_Name = {};
		str::Guid m_Guid = {};

		NS_DECLARE_REFLECTION(gui::VMColour, Noesis::BaseComponent)
	};
}