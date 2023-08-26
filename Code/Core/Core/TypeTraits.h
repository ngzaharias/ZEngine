#pragma once

// https://en.cppreference.com/w/cpp/experimental/is_detected
// https://riptutorial.com/cplusplus/example/18585/is-detected

#include <type_traits>

namespace detail 
{
	template<class Default, class AlwaysVoid, template<class...> class Op, class... Args>
	struct Detector
	{
		using DETECTED = std::false_type;
		using Type = Default;
	};

	template<class Default, template<class...> class Op, class... Args>
	struct Detector<Default, std::void_t<Op<Args...>>, Op, Args...>
	{
		using DETECTED = std::true_type;
		using Type = Op<Args...>;
	};
} 

namespace core
{
	struct NotAType {};

	template<template<class...> class Op, class... Args>
	using IsDetected = typename detail::Detector<NotAType, void, Op, Args...>::DETECTED;

	template<typename Test, template<typename...> class Ref>
	struct IsSpecialization : std::false_type {};

	template<template<typename...> class Ref, typename... Args>
	struct IsSpecialization<Ref<Args...>, Ref> : std::true_type {};

	template <typename TType>
	constexpr bool Contains()
	{
		return false;
	}

	template <typename TType, typename THead, typename... TTail>
	constexpr bool Contains()
	{
		using TStrippedType = typename std::remove_const<TType>::type;
		using TStrippedHead = typename std::remove_const<THead>::type;
		return std::is_same<TStrippedType, TStrippedHead>::value || Contains<TType, TTail...>();
	}
}