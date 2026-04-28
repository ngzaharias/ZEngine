#pragma once

#include "Core/Colour.h"
#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/String.h"
#include "Core/Types.h"

namespace ngraph
{
	struct Field
	{
		int32 m_GraphId = 0;
		str::Guid m_UUID = {};

		Colour m_Colour = {};
		str::Name m_Type = {};
		str::String m_Label = {};
		str::String m_Tooltip = {};
	};

	struct Node
	{
		int32 m_GraphId = 0;
		str::Guid m_UUID = {};

		Colour m_Colour = {};
		str::String m_Label = {};
		str::String m_Tooltip = {};

		Array<Field> m_Inputs = {};
		Array<Field> m_Outputs = {};
	};
}