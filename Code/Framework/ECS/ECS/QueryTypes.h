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
		typename TAdded = TypeList<>,
		typename TRemoved = TypeList<>,
		typename TUpdated = TypeList<>,
		typename TInclude = TypeList<>,
		typename TOptional = TypeList<>,
		typename TExclude = TypeList<>,
		typename TCondition = TypeList<>>
	struct Builder
	{
		template<typename... Types>
		using Added = Builder<decltype(TAdded::template Append<Types...>()), TRemoved, TUpdated, TInclude, TOptional, TExclude, TCondition>;

		template<typename... Types>
		using Removed = Builder<TAdded, decltype(TRemoved::template Append<Types...>()), TUpdated, TInclude, TOptional, TExclude, TCondition>;

		template<typename... Types>
		using Updated = Builder<TAdded, TRemoved, decltype(TUpdated::template Append<Types...>()), TInclude, TOptional, TExclude, TCondition>;

		template<typename... Types>
		using Include = Builder<TAdded, TRemoved, TUpdated, decltype(TInclude::template Append<Types...>()), TExclude, TCondition>;

		template<typename... Types>
		using Optional = Builder<TAdded, TRemoved, TUpdated, TInclude, decltype(TOptional::template Append<Types...>()), TExclude, TCondition>;

		template<typename... Types>
		using Exclude = Builder<TAdded, TRemoved, TUpdated, TInclude, TOptional, decltype(TExclude::template Append<Types...>()), TCondition>;

		template<typename... Types>
		using Condition = Builder<TAdded, TRemoved, TUpdated, TInclude, TOptional, TExclude, decltype(TCondition::template Append<Types...>())>;
	};

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

	template<typename... Types>
	using Condition = Builder<>::Condition<Types...>;

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	// Wrapper to access the TypeLists that makeup a query inside a Builder.
	template<typename TQuery>
	struct Access;

	template<typename TAdded, typename TRemoved, typename TUpdated, typename TInclude, typename TOptional, typename TExclude, typename TCondition>
	struct Access<Builder<TAdded, TRemoved, TUpdated, TInclude, TOptional, TExclude, TCondition>>
	{
		using Added = TAdded;
		using Removed = TRemoved;
		using Updated = TUpdated;
		using Include = TInclude;
		using Optional = TOptional;
		using Exclude = TExclude;
		using Condition = TCondition;
	};

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
	template<typename TQuery>
	using ConditionAccess = typename Access<TQuery>::Condition;
}