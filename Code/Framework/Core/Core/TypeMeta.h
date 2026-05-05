#pragma once



template<typename T>
struct TypeMeta;

template<typename T, T Field>
struct FieldMeta_t
{
	static constexpr inline T GetField() { return Field; }
};

template<auto Field, typename T = decltype(Field)>
using FieldMeta = FieldMeta_t<T, Field>;