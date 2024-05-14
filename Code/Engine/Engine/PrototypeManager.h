#pragma once

#include <Core/Algorithms.h>
#include <Core/Assert.h>
#include <Core/Guid.h>
#include <Core/Map.h>
#include <Core/Name.h>

#include <ECS/Component.h>

#include <Engine/PrototypeLoader.h>
#include <Engine/TypeInfo.h>
#include <Engine/Visitor.h>

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace eng
{
	class AssetManager;
	class Path;
	class Visitor;
	struct Prototype;

	struct PrototypeComponent : public ecs::Component<PrototypeComponent>
	{
		str::Guid m_Guid = {};
	};

	struct PrototypeEntry
	{
		using Add = void(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::Prototype* prototype, const eng::PrototypeLoader* loader);
		using Load = void(eng::Prototype* prototype, const eng::PrototypeLoader* loader, eng::Visitor& visitor);
		using New = eng::Prototype*();

		str::Name m_Type = { };
		eng::PrototypeLoader* m_Loader = nullptr;

		Add* m_Add = nullptr;
		Load* m_Load = nullptr;
		New* m_New = nullptr;
	};

	class PrototypeManager final
	{
		using FileMap = Map<str::Guid, eng::Prototype*>;
		using EntryMap = Map<TypeId, eng::PrototypeEntry>;
		using TypeMap = Map<str::Name, TypeId>;

	public:
		PrototypeManager(eng::AssetManager& assetManager);

		bool CreateEntity(ecs::EntityWorld& world, const ecs::Entity& entity, const str::Path& filepath);

		template<typename TPrototype, typename TLoader, typename... TArgs>
		void RegisterPrototype(const str::Name& type, TArgs&&... args);

	private:
		template<typename TPrototype>
		static eng::Prototype* NewFunction();
		template<typename TPrototype, typename TLoader>
		static void LoadFunction(eng::Prototype* prototype, const eng::PrototypeLoader* loader, eng::Visitor& visitor);
		template<typename TPrototype, typename TLoader>
		static void AddFunction(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::Prototype* prototype, const eng::PrototypeLoader* loader);

	private:
		eng::AssetManager& m_AssetManager;

		FileMap m_FileMap = { };
		EntryMap m_EntryMap = { };
		TypeMap m_TypeMap = { };
	};
}

#include "PrototypeManager.inl"