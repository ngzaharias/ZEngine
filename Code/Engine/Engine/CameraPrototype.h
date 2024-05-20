#pragma once

#include <Engine/CameraTypes.h>
#include <Engine/PrototypeLoader.h>

namespace ecs
{
	class EntityWorld;

	struct Entity;
}

namespace eng
{
	class Visitor;

	struct CameraPrototype final
	{
		camera::Projection m_Projection = camera::Perspective();
	};

	class CameraLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::CameraPrototype& prototype) const;
		void Load(eng::CameraPrototype& prototype, eng::Visitor& visitor) const;
	};
}