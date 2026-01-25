#pragma once

#include "Core/SparseArray.h"
#include "ECS/Entity.h"

namespace ecs
{
	class IComponentContainer
	{
		friend class EntityWorld;

	public:
		virtual ~IComponentContainer() = default;

		virtual bool Contains(const ecs::Entity& entity) const = 0;

		virtual int32 GetCount() const = 0;

		virtual void Move(const ecs::Entity& entity, IComponentContainer& destination) = 0;

		virtual void MoveAll(IComponentContainer& destination) = 0;

		virtual void Remove(const Entity& entity) = 0;
		virtual void RemoveAll() = 0;
	};

	template<typename TComponent>
	class ComponentContainer : public IComponentContainer
	{
		friend class EntityWorld;

	public:
		~ComponentContainer() override = default;

		template<typename... TArgs>
		inline TComponent& Emplace(const ecs::Entity& entity, TArgs&& ...args);

		inline bool Contains(const ecs::Entity& entity) const override;

		inline int32 GetCount() const;

		inline TComponent& Get(const ecs::Entity& entity);
		inline const TComponent& Get(const ecs::Entity& entity) const;

		inline Array<TComponent>& GetAll();
		inline const Array<TComponent>& GetAll() const;

		inline TComponent* Try(const ecs::Entity& entity);
		inline const TComponent* Try(const ecs::Entity& entity) const;

		inline TComponent& Set(const ecs::Entity& entity, TComponent&& value);
		inline TComponent& Set(const ecs::Entity& entity, const TComponent& value);

		inline void Move(const ecs::Entity& entity, IComponentContainer& destination) override;
		inline void MoveAll(IComponentContainer& destination) override;

		inline void Remove(const Entity& entity) override;
		inline void RemoveAll() override;

	private:
		SparseArray<Entity, TComponent> m_Data = { };
	};
}

#include "ComponentContainer.inl"