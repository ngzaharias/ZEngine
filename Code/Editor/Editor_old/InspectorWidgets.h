#pragma once

namespace editor
{
	template<typename Type>
	void InspectType(Type& value);

	template<typename Type>
	void InspectProperty(Type& value);

	template<typename Type>
	void InspectClass(Type& value);
}

#include "InspectorWidgets.inl"