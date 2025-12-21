#pragma once

#include "Core/TypeList.h"
#include "ECS/Entity.h"

#include <tuple>

namespace ecs
{
	class EntityWorld;

	template<typename...>
	class EntityView_t;

	template<typename ...TRequired, typename ...TOptional>
	class EntityView_t<TypeList<TRequired...>, TypeList<TOptional...>>
	{
		friend class EntityWorld;

	private:
		using TRequiredList = TypeList<TRequired...>;
		using TOptionalList = TypeList<TOptional...>;

	public:
		template <typename... Types>
		using Required = EntityView_t<decltype(TRequiredList::template Append<Types...>()), TOptionalList>;
		template <typename... Types>
		using Optional = EntityView_t<TRequiredList, decltype(TOptionalList::template Append<Types...>())>;

		EntityView_t(const ecs::Entity& entity, ecs::EntityWorld& world);

		operator ecs::Entity() { return m_Entity; }

		const ecs::Entity& GetEntity() const
		{
			return m_Entity;
		}

		template<typename TComponent>
		auto ReadRequired() const -> const TComponent&;

		template<typename TComponent>
		auto WriteRequired()->TComponent&;

		template<typename TComponent>
		bool HasOptional() const;

		template<typename TComponent>
		auto ReadOptional() const -> const TComponent*;

		template<typename TComponent>
		auto WriteOptional() -> TComponent*;

	private:
		ecs::Entity m_Entity;
		ecs::EntityWorld& m_World;

		std::tuple<TRequired*...> m_Required;
		std::tuple<TOptional*...> m_Optional;
	};

	using EntityView = ecs::EntityView_t<TypeList<>, TypeList<>>;
}

#include "ECS/EntityView.inl"