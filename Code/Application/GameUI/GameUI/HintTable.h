#pragma once

#include "Core/Guid.h"
#include "Core/String.h"
#include "Engine/TableManager.h"

namespace gui
{
	struct Hint
	{
		str::String m_Text = {};
	};

	class HintTable final : public eng::TableManager<str::Guid, gui::Hint> 
	{ 
	public:
		const Hint& GetRandom() const;
	};
}