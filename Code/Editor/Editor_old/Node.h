#pragma once

#include <Core/Array.h>
#include <Core/String.h>

namespace node
{
	struct Node
	{
		str::String m_Label = { };

		int32 m_Handle = -1;
		Array<int32> m_Inputs = { };
		Array<int32> m_Outputs = { };
	};
}