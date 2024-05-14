#pragma once

#include <ECS/Entity.h>
#include <ECS/System.h>

namespace str
{
	class Path;
	class Name;
}

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class PrototypeManager;
	struct LevelDirectoryComponent;
	struct LevelEntityComponent;
	struct LevelLoadedComponent;
	struct LevelLoadRequestComponent;
	struct LevelUnloadRequestComponent;

	class LevelSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// managers
			eng::PrototypeManager,
			// components
			ecs::NameComponent,
			eng::LevelDirectoryComponent,
			eng::LevelEntityComponent,
			eng::LevelLoadedComponent,
			eng::LevelLoadRequestComponent,
			eng::LevelUnloadRequestComponent>;
		
		LevelSystem(ecs::EntityWorld& entityWorld);

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