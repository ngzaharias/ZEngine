#pragma once

#include "Core/Map.h"
#include "Core/Name.h"
#include "Engine/PrototypeComponent.h"
#include "imgui/Inspector.h"

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
		using Save = void(ecs::EntityWorld&, const ecs::Entity&, eng::Visitor&);
		Save* m_Save = nullptr;

		using Load = void(ecs::EntityWorld&, const ecs::Entity&, eng::Visitor&);
		Load* m_Load = nullptr;

		using Inspect = void(ecs::EntityWorld&, const ecs::Entity&, imgui::Inspector&);
		Inspect* m_Inspect = nullptr;
	};

	class PrototypeManager final
	{
	public:
		static constexpr const char* s_Extension = ".prototype";

		using EntryMap = Map<str::Name, eng::PrototypeEntry>;

	public:
		template<typename TComponent>
		void RegisterComponent();

		bool SaveEntity(ecs::EntityWorld& world, const ecs::Entity& entity, const str::Path& filepath) const;
		bool LoadEntity(ecs::EntityWorld& world, const ecs::Entity& entity, const str::Path& filepath) const;
		void InspectEntity(ecs::EntityWorld& world, const ecs::Entity& entity, imgui::Inspector& inspector) const;

	private:
		template<typename TComponent>
		static void SaveComponent(ecs::EntityWorld& world, const ecs::Entity& entity, eng::Visitor& visitor);
		template<typename TComponent>
		static void LoadComponent(ecs::EntityWorld& world, const ecs::Entity& entity, eng::Visitor& visitor);
		template<typename TComponent>
		static void InspectComponent(ecs::EntityWorld& world, const ecs::Entity& entity, imgui::Inspector& insepctor);

	private:
		EntryMap m_EntryMap = { };
	};
}

#include "PrototypeManager.inl"