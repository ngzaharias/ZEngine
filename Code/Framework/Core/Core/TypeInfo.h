#pragma once

#include "Core/Array.h"
#include "Core/Hash.h"
#include "Core/Set.h"
#include "Core/TypeList.h"
#include "Core/Types.h"

using TypeHash = uint32;
using TypeId = int32;
struct TypeTag {};

namespace _private
{
	struct TypeIndex
	{
		template<typename Tag>
		[[nodiscard]] static TypeId Next() noexcept
		{
			static TypeId value = 0;
			return value++;
		}
	};
}

template<typename Type>
constexpr TypeHash ToTypeHash()
{
	return str::ToHash(__FUNCSIG__);
}

template<typename Type>
constexpr TypeHash ToTypeHash(const Type& value)
{
	return str::ToHash(__FUNCSIG__);
}

template<typename Type, typename Tag>
TypeId ToTypeId()
{
	static const TypeId value = _private::TypeIndex::Next<Tag>();
	return value;
}

template <typename... TTypes>
void ToTypeId(Set<TypeId>& values, TypeList<TTypes...>)
{
	(values.Add(ToTypeId<TTypes, TypeTag>()), ...);
}

template<typename Type>
const char* ToTypeName()
{
	// #note: MSVC returns "class <TYPE>", "struct <TYPE>" or "<TYPE>"
	// will also include namespace (including anonymous).
	static const char* value = typeid(Type).name();
	return value;
}
