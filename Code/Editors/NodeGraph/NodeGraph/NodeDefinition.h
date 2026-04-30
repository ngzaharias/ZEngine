#pragma once

#include "Core/Array.h"
#include "Core/Colour.h"
#include "Core/Name.h"
#include "Core/Optional.h"
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
		str::String m_Label = {};
		str::String m_Tooltip = {};
		Optional<Colour> m_Colour = {};

		Array<FieldDefinition> m_Inputs = {};
		Array<FieldDefinition> m_Outputs = {};
	};
}