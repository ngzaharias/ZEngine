#pragma once

#include "Core/Map.h"
#include "Core/Name.h"
#include "Engine/PrototypeComponent.h"

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace eng
{
	class Visitor;
	struct Prototype;
}

namespace eng
{
	struct PrototypeEntry
	{
		using Save = void(ecs::EntityWorld& world, const ecs::Entity& entity, eng::Visitor& visitor);
		using Load = void(ecs::EntityWorld& world, const ecs::Entity& entity, eng::Visitor& visitor);

		Save* m_Save = nullptr;
		Load* m_Load = nullptr;
	};

	class PrototypeManager final
	{
		using EntryMap = Map<str::Name, eng::PrototypeEntry>;

	public:
		template<typename TPrototype>
		void Register();

		bool SaveEntity(ecs::EntityWorld& world, const ecs::Entity& entity, const str::Path& filepath) const;
		bool LoadEntity(ecs::EntityWorld& world, const ecs::Entity& entity, const str::Path& filepath) const;

	private:
		template<typename TPrototype>
		static void SaveFunction(ecs::EntityWorld& world, const ecs::Entity& entity, eng::Visitor& visitor);
		template<typename TPrototype>
		static void LoadFunction(ecs::EntityWorld& world, const ecs::Entity& entity, eng::Visitor& visitor);

	private:
		EntryMap m_EntryMap = { };
	};
}

#include "PrototypeManager.inl"