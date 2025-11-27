#pragma once

#include "Core/Map.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "ECS/Component.h"
#include "ECS/Singleton.h"

namespace eng::level
{
	enum class ELoadingState
	{
		FadeOut,
		Unload,
		Load,
		FadeIn,
	};

	/// \brief Contains every available level that can be loaded.
	struct DirectorySingleton : public ecs::Singleton<DirectorySingleton>
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

	/// \brief There exists one of these components for every level that is loaded.
	struct LoadingComponent : public ecs::Component<LoadingComponent>
	{
		str::Name m_Name = {};

		ELoadingState m_StateCurrent = ELoadingState::FadeOut;
		ELoadingState m_StatePrevious = ELoadingState::FadeOut;
		float m_FadeInTime = 1.f;
		float m_FadeInTimer = 0.f;
		float m_FadeOutTime = 1.f;
		float m_FadeOutTimer = 0.f;
		bool m_IsSplash = false;
	};

	/// \brief Request to load a specific level.
	struct LoadRequest : public ecs::Event<LoadRequest>
	{
		LoadRequest() = default;
		LoadRequest(const str::Name& name) : m_Name(name) { }
		str::Name m_Name = {};
		bool m_IsSplash = false;
	}; 
}