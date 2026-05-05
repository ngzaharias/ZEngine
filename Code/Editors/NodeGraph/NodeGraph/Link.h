#pragma once

#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/Types.h"

namespace ngraph
{
	struct Link
	{
		str::Guid m_UUID = {};
		str::Name m_Type = {};
		int32 m_SourceId = 0;
		int32 m_TargetId = 0;
	};
}