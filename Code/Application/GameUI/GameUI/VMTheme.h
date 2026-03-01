#pragma once

#include "Core/Guid.h"
#include "Core/Name.h"
#include "Engine/UIViewModel.h"

namespace gui
{
	class VMTheme final : public eng::UIViewModel
	{
	public:
		VMTheme(const str::Name& name, const str::Guid& guid);

		const char* GetName() const;

	public:
		str::Name m_Name = {};
		str::Guid m_Guid = {};

	private:
		NS_DECLARE_REFLECTION(gui::VMTheme, Noesis::BaseComponent)
	};
}