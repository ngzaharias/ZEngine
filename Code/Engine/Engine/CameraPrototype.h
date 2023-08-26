#pragma once

#include <Engine/Prototype.h>
#include <Engine/PrototypeLoader.h>
#include <Engine/TomlTypes.h>

namespace ecs
{
	class EntityWorld;

	struct Entity;
}

namespace eng
{
	class Visitor;

	struct CameraPrototype final : public eng::Prototype
	{
	};

	class CameraPrototypeLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const CameraPrototype& prototype) const;
		void Load(CameraPrototype& prototype, eng::Visitor& visitor) const;
	};
}