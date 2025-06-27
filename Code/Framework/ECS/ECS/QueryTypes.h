#pragma once

template <typename... Types>
struct TypeList;

namespace ecs
{
	struct QueryTag {};
}

namespace ecs::query
{
	template<
		typename TAdded = TypeList<>,
		typename TRemoved = TypeList<>,
		typename TUpdated = TypeList<>,
		typename TInclude = TypeList<>,
		typename TExclude = TypeList<>>
	struct Builder
	{
		template <typename... Types>
		using Added = Builder<decltype(TAdded::template Append<Types...>()), TRemoved, TUpdated, TInclude, TExclude>;

		template <typename... Types>
		using Removed = Builder<TAdded, decltype(TRemoved::template Append<Types...>()), TUpdated, TInclude, TExclude>;

		template <typename... Types>
		using Updated = Builder<TAdded, TRemoved, decltype(TUpdated::template Append<Types...>()), TInclude, TExclude>;

		template <typename... Types>
		using Include = Builder<TAdded, TRemoved, TUpdated, decltype(TInclude::template Append<Types...>()), TExclude>;

		template <typename... Types>
		using Exclude = Builder<TAdded, TRemoved, TUpdated, TInclude, decltype(TExclude::template Append<Types...>())>;
	};

	template <typename... Types>
	using Added = Builder<>::Added<Types...>;

	template <typename... Types>
	using Removed = Builder<>::Removed<Types...>;

	template <typename... Types>
	using Updated = Builder<>::Updated<Types...>;

	template <typename... Types>
	using Include = Builder<>::Include<Types...>;

	template <typename... Types>
	using Exclude = Builder<>::Exclude<Types...>;

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	// Wrapper to access the TypeLists that makeup a query inside a Builder.
	template <typename TQuery>
	struct Access;

	template <typename TAdded, typename TRemoved, typename TUpdated, typename TInclude, typename TExclude>
	struct Access<Builder<TAdded, TRemoved, TUpdated, TInclude, TExclude>>
	{
		using Added = TAdded;
		using Removed = TRemoved;
		using Updated = TUpdated;
		using Include = TInclude;
		using Exclude = TExclude;
	};

	template <typename TQuery>
	using AddedAccess = typename Access<TQuery>::Added;
	template <typename TQuery>
	using RemovedAccess = typename Access<TQuery>::Removed;
	template <typename TQuery>
	using UpdatedAccess = typename Access<TQuery>::Updated;
	template <typename TQuery>
	using IncludeAccess = typename Access<TQuery>::Include;
	template <typename TQuery>
	using ExcludeAccess = typename Access<TQuery>::Exclude;
}