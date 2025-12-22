#pragma once

template <typename... Types>
struct TypeList
{
	template <typename... Added>
	using Append = TypeList<Types..., Added...>;

	using NonConst = TypeList<std::remove_const_t<Types>...>;

};

template<typename...>
struct TypeMerge;

template<typename ...A, typename ...B>
struct TypeMerge<TypeList<A...>, TypeList<B...>>
{
	using TypeList = TypeList<A..., B...>;
};
