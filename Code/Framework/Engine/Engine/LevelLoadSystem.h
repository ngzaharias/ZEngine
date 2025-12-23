#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class PrototypeManager;
}

namespace eng::level
{
	struct DirectorySingleton;
	struct EntityComponent;
	struct LoadedComponent;
	struct LoadingComponent;
	struct LoadRequest;
}

namespace str
{
	class Path;
	class Name;
}

namespace eng::level
{
	class LoadSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			eng::level::DirectorySingleton,
			eng::level::EntityComponent,
			eng::level::LoadedComponent,
			eng::level::LoadingComponent,
			eng::level::LoadRequest,
			eng::PrototypeManager>;
		
		LoadSystem(ecs::EntityWorld& entityWorld);

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		void LoadLevel(World& world, const str::Name& name, const str::Path& directory);
		void UnloadLevel(World& world, const str::Name& name);

	private:
		ecs::EntityWorld& m_EntityWorld;

		ecs::Entity m_Entity = { };
	};
};