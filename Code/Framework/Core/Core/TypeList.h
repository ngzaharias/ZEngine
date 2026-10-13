#pragma once

#include <concepts>
#include <type_traits>

template<typename... Types>
struct TypeList
{
	template<typename... Added>
	using Append = TypeList<Types..., Added...>;

	using NonConst = TypeList<std::remove_const_t<Types>...>;

	static constexpr bool IsEmpty = sizeof...(Types) == 0;
};

template<typename...>
struct TypeMerge;

template<typename ...A, typename ...B>
struct TypeMerge<TypeList<A...>, TypeList<B...>>
{
	using TypeList = TypeList<A..., B...>;
};

template<typename List>
struct TypeConst;

template<>
struct TypeConst<TypeList<>>
{
	using type = TypeList<>;
};

template<typename Head, typename... Tail>
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

template<typename List>
struct TypeNonConst;

template<>
struct TypeNonConst<TypeList<>>
{
	using type = TypeList<>;
};

template<typename Head, typename... Tail>
struct TypeNonConst<TypeList<Head, Tail...>>
{
private:
	using List = typename TypeNonConst<TypeList<Tail...>>::type;

public:
	using type = std::conditional_t<
		!std::is_const_v<Head>,
		typename TypeMerge<TypeList<Head>, List>::TypeList,
		List>;
}; 

template<typename Base, typename List>
struct TypeDerived;

template<typename Base>
struct TypeDerived<Base, TypeList<>>
{
	using type = TypeList<>;
};

template<typename Base, typename Head, typename... Tail>
struct TypeDerived<Base, TypeList<Head, Tail...>>
{
private:
	using List = typename TypeDerived<Base, TypeList<Tail...>>::type;

public:
	using type = std::conditional_t<
		std::derived_from<Head, Base>,
		typename TypeMerge<TypeList<Head>, List>::TypeList,
		List>;
};