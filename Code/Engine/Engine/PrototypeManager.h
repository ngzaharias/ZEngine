#pragma once

#include "Core/Guid.h"
#include "Core/Map.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace ecs
{
	class EntityWorld;
}

namespace eng
{
	class PrototypeLoader;
	class Visitor;
	struct Prototype;
}

namespace eng
{
	struct PrototypeComponent : public ecs::Component<PrototypeComponent>
	{
		str::Guid m_Guid = {};
		str::Path m_Path = { };
	};

	struct PrototypeEntry
	{
		using Create = void(eng::Visitor& visitor, ecs::EntityWorld& world, const ecs::Entity& entity, const eng::PrototypeLoader& loader);

		Create* m_Create = nullptr;
		eng::PrototypeLoader* m_Loader = nullptr;
	};

	class PrototypeManager final
	{
		using EntryMap = Map<str::Name, eng::PrototypeEntry>;

	public:
		PrototypeManager();
		~PrototypeManager();

		ecs::Entity CreateEntity(ecs::EntityWorld& world, const str::Path& filepath) const;

		template<typename TPrototype, typename TLoader, typename... TArgs>
		void RegisterPrototype(const str::Name& type, TArgs&&... args);

	private:
		template<typename TPrototype, typename TLoader>
		static void CreateFunction(eng::Visitor& visitor, ecs::EntityWorld& world, const ecs::Entity& entity, const eng::PrototypeLoader& loader);

	private:
		EntryMap m_EntryMap = { };
	};
}

#include "PrototypeManager.inl"