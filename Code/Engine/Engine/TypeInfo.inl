#pragma once

#include <entt/core/type_info.hpp>

template<typename Type>
constexpr TypeId ToTypeId()
{
	return entt::type_info<Type>::id();
}

template<typename Type>
constexpr TypeId ToTypeId(const Type& value)
{
	return entt::type_info<Type>::id();
}