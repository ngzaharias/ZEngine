#pragma once

#include "Core/Colour.h"
#include "Core/Guid.h"
#include "Core/Name.h"
#include "Engine/TableManager.h"

namespace eng
{
	struct Theme
	{
		str::Name m_Name = {};
		Colour m_Background0 = {};
		Colour m_Background1 = {};
		Colour m_Background2 = {};
		Colour m_Highlight = {};
		Colour m_Outline = {};
	};

	class ThemeTable final : public eng::TableManager<str::Guid, eng::Theme>
	{
	public:
		const eng::Theme& GetTheme(const str::Guid& value) const;
	};
}