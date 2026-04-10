#pragma once

#include "Core/Map.h"
#include "Core/Name.h"
#include "Core/TypeName.h"
#include "ECS/Component.h"
#include "ECS/EntityWorld.h"
#include "Serialize/Visitor.h"

#include <type_traits>

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace eng
{
	struct TemplateEntry
	{
		using Read = void(ecs::EntityWorld&, const ecs::Entity&, Visitor&);
		Read* m_Read = nullptr;
		using Write = void(ecs::EntityWorld&, const ecs::Entity&, Visitor&);
		Write* m_Write = nullptr;
	};

	class TemplateManager final
	{
	public:
		static constexpr const char* s_Extension = ".template";

		using EntryMap = Map<str::Name, eng::TemplateEntry>;

	public:
		template<typename TComponent>
		void RegisterComponent();

		void ReadEntity(ecs::EntityWorld& world, const ecs::Entity& entity, Visitor& visitor) const;
		void WriteEntity(ecs::EntityWorld& world, const ecs::Entity& entity, Visitor& visitor) const;

	private:
		template<typename TComponent>
		static void ReadComponent(ecs::EntityWorld& world, const ecs::Entity& entity, Visitor& visitor);
		template<typename TComponent>
		static void WriteComponent(ecs::EntityWorld& world, const ecs::Entity& entity, Visitor& visitor);

	private:
		EntryMap m_EntryMap = { };
	};
}

#include "TemplateManager.inl"