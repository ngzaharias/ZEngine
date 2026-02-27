#pragma once

#include "Core/Guid.h"
#include "Engine/UIViewModel.h"

namespace str
{
	class Name;
}

namespace gui
{
	class VMColour final : public eng::UIViewModel
	{
	public:
		VMColour(const str::Name& name, const str::Guid& guid);

		const char* GetName() const;

	public:
		Noesis::String m_Name = {};
		str::Guid m_Guid = {};

	private:
		NS_DECLARE_REFLECTION(gui::VMColour, Noesis::BaseComponent)
	};
}