#pragma once

#include <Core/Variant.h>

namespace editor
{
	template<typename Type, typename TVariant>
	void InspectVariant(TVariant& variant, Type& value);

	template<typename Type, typename ...Types>
	void InspectVariant(Variant<Types...>& variant, Type& value);
}

#include "VariantWidgets.inl"