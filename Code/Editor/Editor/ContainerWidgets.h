#pragma once

#include <Core/Array.h>
#include <Core/Map.h>
#include <Core/Set.h>

namespace editor
{
	template<class Container>
	void InspectContainer(Container& container);

	template<typename Type>
	void InspectContainer(Array<Type>& container);

	template<class Key, class Value>
	void InspectContainer(Map<Key, Value>& container);

	template<typename Type>
	void InspectContainer(Set<Type>& container);
}

#include "ContainerWidgets.inl"