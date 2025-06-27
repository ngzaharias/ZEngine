#pragma once

#include "Core/Hash.h"

template<typename Type>
constexpr TypeId ToTypeId()
{
	return str::ToHash(__FUNCSIG__);
}

template<typename Type>
constexpr TypeId ToTypeId(const Type& value)
{
	return str::ToHash(__FUNCSIG__);
}