#pragma once

#include "Core/Map.h"
#include "Core/Name.h"
#include "Core/TypeName.h"
#include "ECS/Component.h"
#include "ECS/EntityWorld.h"
#include "Engine/TemplateComponent.h"
#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

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
		using Load = void(ecs::EntityWorld&, const ecs::Entity&, Visitor&);
		Load* m_Load = nullptr;
		using Read = void(ecs::EntityWorld&, const ecs::Entity&, Visitor&);
		Read* m_Read = nullptr;
		using Sync = void(ecs::EntityWorld&, const ecs::Entity&, Visitor&);
		Sync* m_Sync = nullptr;

		using Inspect = bool(ecs::EntityWorld&, const ecs::Entity&, imgui::Inspector&);
		Inspect* m_Inspect = nullptr;
	};

	class TemplateManager final
	{
	public:
		static constexpr const char* s_Extension = ".template";

		using EntryMap = Map<str::Name, eng::TemplateEntry>;

	public:
		template<typename TComponent>
		void RegisterComponent();

		void ReadEntity(ecs::EntityWorld& world, const ecs::Entity& entity, str::String& data) const;
		void LoadEntity(ecs::EntityWorld& world, const ecs::Entity& entity, const str::String& data) const;
		void SyncEntity(ecs::EntityWorld& world, const ecs::Entity& entity, const str::String& data) const;

		bool InspectEntity(ecs::EntityWorld& world, const ecs::Entity& entity, imgui::Inspector& inspector) const;

	private:
		template<typename TComponent>
		static void ReadComponent(ecs::EntityWorld& world, const ecs::Entity& entity, Visitor& visitor);
		template<typename TComponent>
		static void LoadComponent(ecs::EntityWorld& world, const ecs::Entity& entity, Visitor& visitor);
		template<typename TComponent>
		static void SyncComponent(ecs::EntityWorld& world, const ecs::Entity& entity, Visitor& visitor);

		template<typename TComponent>
		static bool InspectComponent(ecs::EntityWorld& world, const ecs::Entity& entity, imgui::Inspector& insepctor);

	private:
		EntryMap m_EntryMap = { };
	};
}

#include "TemplateManager.inl"