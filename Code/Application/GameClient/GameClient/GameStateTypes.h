#pragma once

#include "Core/Name.h"
#include "Core/Variant.h"
#include "Network/Types.h"

namespace gamestate
{
	struct None { };

	struct LevelLoad
	{
		str::Name m_Name = { };
	};

	struct LevelUnload
	{ 
		str::Name m_Name = { };
	};

	using State = Variant<
		None,
		LevelLoad,
		LevelUnload>;
}