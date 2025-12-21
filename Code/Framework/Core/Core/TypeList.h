#pragma once

template <typename... Types>
struct TypeList
{
	template <typename... Added>
	using Append = TypeList<Types..., Added...>;

	using NonConst = TypeList<std::remove_const_t<Types>...>;
};