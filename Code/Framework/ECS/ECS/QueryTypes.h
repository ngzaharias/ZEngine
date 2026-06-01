#pragma once

template<typename... Types>
struct TypeList;

namespace ecs
{
	struct Alive {};
	struct Dead {};
	struct QueryTag {};
}

namespace ecs::query
{
	template<
		typename TCondition = TypeList<>,
		typename TAdded = TypeList<>,
		typename TRemoved = TypeList<>,
		typename TUpdated = TypeList<>,
		typename TInclude = TypeList<>,
		typename TOptional = TypeList<>,
		typename TExclude = TypeList<>>
	struct Builder
	{
		template<typename... Types>
		using Condition = Builder<decltype(TCondition::template Append<Types...>()), TAdded, TRemoved, TUpdated, TInclude, TOptional, TExclude>;

		template<typename... Types>
		using Added = Builder<TCondition, decltype(TAdded::template Append<Types...>()), TUpdated, TInclude, TOptional, TExclude, TExclude>;

		template<typename... Types>
		using Removed = Builder<TCondition, TAdded, decltype(TRemoved::template Append<Types...>()), TInclude, TOptional, TExclude, TExclude>;

		template<typename... Types>
		using Updated = Builder<TCondition, TAdded, TRemoved, decltype(TUpdated::template Append<Types...>()), TExclude, TExclude>;

		template<typename... Types>
		using Include = Builder<TCondition, TAdded, TRemoved, TUpdated, decltype(TInclude::template Append<Types...>()), TExclude, TExclude>;

		template<typename... Types>
		using Optional = Builder<TCondition, TAdded, TRemoved, TUpdated, TInclude, decltype(TOptional::template Append<Types...>()), TExclude>;

		template<typename... Types>
		using Exclude = Builder<TCondition, TAdded, TRemoved, TUpdated, TInclude, TOptional, decltype(TExclude::template Append<Types...>())>;
	};

	template<typename... Types>
	using Condition = Builder<>::Condition<Types...>;

	template<typename... Types>
	using Added = Builder<>::Added<Types...>;

	template<typename... Types>
	using Removed = Builder<>::Removed<Types...>;

	template<typename... Types>
	using Updated = Builder<>::Updated<Types...>;

	template<typename... Types>
	using Include = Builder<>::Include<Types...>;

	template<typename... Types>
	using Optional = Builder<>::Optional<Types...>;

	template<typename... Types>
	using Exclude = Builder<>::Exclude<Types...>;

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	// Wrapper to access the TypeLists that makeup a query inside a Builder.
	template<typename TQuery>
	struct Access;

	template<typename TCondition, typename TAdded, typename TRemoved, typename TUpdated, typename TInclude, typename TOptional, typename TExclude>
	struct Access<Builder<TCondition, TAdded, TRemoved, TUpdated, TInclude, TOptional, TExclude>>
	{
		using Condition = TCondition;
		using Added = TAdded;
		using Removed = TRemoved;
		using Updated = TUpdated;
		using Include = TInclude;
		using Optional = TOptional;
		using Exclude = TExclude;
	};

	template<typename TQuery>
	using ConditionAccess = typename Access<TQuery>::Condition;
	template<typename TQuery>
	using AddedAccess = typename Access<TQuery>::Added;
	template<typename TQuery>
	using RemovedAccess = typename Access<TQuery>::Removed;
	template<typename TQuery>
	using UpdatedAccess = typename Access<TQuery>::Updated;
	template<typename TQuery>
	using IncludeAccess = typename Access<TQuery>::Include;
	template<typename TQuery>
	using OptionalAccess = typename Access<TQuery>::Optional;
	template<typename TQuery>
	using ExcludeAccess = typename Access<TQuery>::Exclude;
}