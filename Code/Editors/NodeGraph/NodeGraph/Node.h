#pragma once

#include "Core/Colour.h"
#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/Optional.h"
#include "Core/String.h"
#include "Core/Types.h"

namespace ngraph
{
	struct Field
	{
		int32 m_GraphId = 0;
		str::Guid m_UUID = {};
		str::Name m_Type = {};

		str::String m_Label = {};
		str::String m_Tooltip = {};
		Optional<Colour> m_Colour = {};
	};

	struct Node
	{
		int32 m_GraphId = 0;
		str::Guid m_UUID = {};
		str::Name m_Type = {};

		str::String m_Label = {};
		str::String m_Tooltip = {};
		Optional<Colour> m_Colour = {};

		Array<Field> m_Inputs = {};
		Array<Field> m_Outputs = {};
	};
}