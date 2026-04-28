#pragma once

#include "Core/Array.h"
#include "Core/Colour.h"
#include "Core/Name.h"
#include "Core/String.h"

namespace ngraph
{
	struct FieldDefinition
	{
		str::Name m_Type = {};
		str::String m_Label = {};
		str::String m_Tooltip = {};
	};

	struct NodeDefinition
	{
		Colour m_Colour = {};
		str::String m_Label = {};
		str::String m_Tooltip = {};

		Array<FieldDefinition> m_Inputs = {};
		Array<FieldDefinition> m_Outputs = {};
	};
}