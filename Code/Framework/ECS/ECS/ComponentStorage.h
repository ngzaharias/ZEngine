#pragma once

#include "Core/SparseArray.h"
#include "ECS/Entity.h"

namespace ecs
{
	class IComponentStorage
	{
		friend class EntityWorld;

	public:
		virtual ~IComponentStorage() = default;

		virtual bool Contains(const ecs::Entity& entity) const = 0;

		virtual void Move(IComponentStorage& destination) = 0;
		virtual void Move(const ecs::Entity& entity, IComponentStorage& destination) = 0;

		virtual void Remove(const Entity& entity) = 0;
		virtual void RemoveAll() = 0;
	};

	template<typename TComponent>
	class ComponentStorage : public IComponentStorage
	{
		friend class EntityWorld;

	public:
		inline bool Contains(const ecs::Entity& entity) const override;

		inline TComponent& Get(const ecs::Entity& entity);
		inline const TComponent& Get(const ecs::Entity& entity) const;

		inline Array<TComponent>& GetValues();
		inline const Array<TComponent>& GetValues() const;

		template<typename... TArgs>
		inline TComponent& Emplace(const ecs::Entity& entity, TArgs&& ...args);

		inline TComponent& Set(const ecs::Entity& entity, TComponent&& value);
		inline TComponent& Set(const ecs::Entity& entity, const TComponent& value);

		inline void Move(IComponentStorage& destination) override;
		inline void Move(const ecs::Entity& entity, IComponentStorage& destination) override;

		inline void Remove(const Entity& entity) override;
		inline void RemoveAll() override;

	private:
		SparseArray<Entity, TComponent> m_Data = { };
	};
}

#include "ComponentStorage.inl"