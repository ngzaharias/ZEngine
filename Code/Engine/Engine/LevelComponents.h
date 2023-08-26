#pragma once

#include <Core/Map.h>
#include <Core/Name.h>
#include <Core/Path.h>

#include <ECS/Component.h>

namespace eng
{
	/// \brief Contains every available level that can be loaded.
	struct LevelDirectoryComponent : public ecs::SingletonComponent<LevelDirectoryComponent>
	{
		Map<str::Name, str::Path> m_Levels;
	};

	/// \brief Attached to entities that were loaded from a level.
	struct LevelEntityComponent : public ecs::Component<LevelEntityComponent>
	{
		LevelEntityComponent() = default;
		LevelEntityComponent(const str::Name & name) : m_Name(name) { }
		str::Name m_Name = { };
	};

	/// \brief There exists one of these components for every level that is loaded.
	struct LevelLoadedComponent : public ecs::Component<LevelLoadedComponent>
	{
		LevelLoadedComponent() = default;
		LevelLoadedComponent(const str::Name & name) : m_Name(name) { }
		str::Name m_Name = { };
	};

	/// \brief Used to request to load a specific level.
	struct LevelLoadRequestComponent : public ecs::EventComponent<LevelLoadRequestComponent>
	{
		LevelLoadRequestComponent() = default;
		LevelLoadRequestComponent(const str::Name& name) : m_Name(name) { }
		str::Name m_Name = { };
	};

	/// \brief Used to request to unload a specific level.
	struct LevelUnloadRequestComponent : public ecs::EventComponent<LevelUnloadRequestComponent>
	{
		LevelUnloadRequestComponent() = default;
		LevelUnloadRequestComponent(const str::Name & name) : m_Name(name) { }
		str::Name m_Name = { };
	};
}