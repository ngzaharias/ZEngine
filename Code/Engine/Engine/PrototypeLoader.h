#pragma once

#include "Core/Guid.h"
#include "Core/Map.h"

namespace ecs
{
	class EntityWorld;
}

namespace eng
{
	class AssetManager;
	class PrototypeManager;
	struct Prototype;

	class PrototypeLoader
	{
		friend class eng::PrototypeManager;
		using Cache = Map<str::Guid, eng::Prototype*>;

	public:
		explicit PrototypeLoader() = default;
		virtual ~PrototypeLoader() = default;

	protected:
		eng::AssetManager* m_AssetManager = nullptr;
		eng::PrototypeManager* m_PrototypeManager = nullptr;

		Cache m_Cache = { };
	};
}