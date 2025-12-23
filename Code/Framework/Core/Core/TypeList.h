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

template <typename List>
struct TypeConst;

template <>
struct TypeConst<TypeList<>>
{
	using type = TypeList<>;
};

template <typename Head, typename... Tail>
struct TypeConst<TypeList<Head, Tail...>>
{
private:
	using List = typename TypeConst<TypeList<Tail...>>::type;

public:
	using type = std::conditional_t<
		std::is_const_v<Head>, 
		typename TypeMerge<TypeList<Head>, List>::TypeList,
		List>;
};