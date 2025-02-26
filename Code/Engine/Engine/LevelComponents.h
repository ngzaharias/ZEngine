#pragma once

#include "Core/Map.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "ECS/Component.h"

namespace eng::level
{
	/// \brief Contains every available level that can be loaded.
	struct DirectoryComponent : public ecs::SingletonComponent<DirectoryComponent>
	{
		Map<str::Name, str::Path> m_Levels;
	};

	/// \brief Attached to entities that were loaded from a level.
	struct EntityComponent : public ecs::Component<EntityComponent>
	{
		EntityComponent() = default;
		EntityComponent(const str::Name & name) : m_Name(name) { }
		str::Name m_Name = {};
	};

	/// \brief There exists one of these components for every level that is loaded.
	struct LoadedComponent : public ecs::Component<LoadedComponent>
	{
		str::Name m_Name = {};
		str::Path m_Path = {};
	};

	/// \brief Request to load a specific level.
	struct LoadRequestComponent : public ecs::EventComponent<LoadRequestComponent>
	{
		LoadRequestComponent() = default;
		LoadRequestComponent(const str::Name& name) : m_Name(name) { }
		str::Name m_Name = {};
	}; 

	/// \brief Request to unload a specific level.
	struct UnloadRequestComponent : public ecs::EventComponent<UnloadRequestComponent>
	{
		UnloadRequestComponent() = default;
		UnloadRequestComponent(const str::Name & name) : m_Name(name) { }
		str::Name m_Name = {};
	};
}