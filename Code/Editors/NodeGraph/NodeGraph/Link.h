#pragma once

#include "Core/Types.h"

namespace ngraph
{
	struct Link
	{
		str::Guid m_UUID = {};
		int32 m_SourceId = 0;
		int32 m_TargetId = 0;
	};
}