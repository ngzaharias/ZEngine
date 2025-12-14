#pragma once

#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/Path.h"

namespace eng
{
	struct AssetFile
	{
		str::Guid m_Guid = { };
		str::Name m_Name = { };
		str::Path m_Path = { };
		str::Name m_Type = { };
	};
}
