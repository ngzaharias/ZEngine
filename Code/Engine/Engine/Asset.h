#pragma once

#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "Engine/TypeInfo.h"

namespace eng
{
	struct Asset
	{
		Asset() = default;
		virtual ~Asset() = default;

		str::Guid m_Guid = { };
		str::Name m_Name = { };
		str::Name m_Type = { };
	};
}