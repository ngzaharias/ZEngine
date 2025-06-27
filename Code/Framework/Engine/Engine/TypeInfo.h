#pragma once

using uint32 = uint32_t;
using TypeId = uint32;

template<typename Type>
constexpr TypeId ToTypeId();

template<typename Type>
constexpr TypeId ToTypeId(const Type& value);

#include "TypeInfo.inl"