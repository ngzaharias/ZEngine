#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"

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
	struct DirectoryComponent;
	struct EntityComponent;
	struct LoadedComponent;
	struct LoadRequestComponent;
	struct UnloadRequestComponent;
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
		using World = ecs::WorldView<
			// managers
			eng::PrototypeManager,
			// components
			ecs::NameComponent,
			eng::level::DirectoryComponent,
			eng::level::EntityComponent,
			eng::level::LoadedComponent,
			eng::level::LoadRequestComponent,
			eng::level::UnloadRequestComponent>;
		
		LoadSystem(ecs::EntityWorld& entityWorld);

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		void LoadLevel(World& world, const str::Name& name, const str::Path& directory);
		void UnloadLevel(World& world, const str::Name& name);

	private:
		ecs::EntityWorld& m_EntityWorld;

		ecs::Entity m_Entity = { };
	};
};