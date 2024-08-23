#pragma once

#include "Engine/CameraTypes.h"
#include "Engine/PrototypeLoader.h"
#include "Math/Math.h"
#include "Math/Vector.h"

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
		camera::EBehaviour m_Behaviour = camera::EBehaviour::Free3D;
		float m_ZoomMax = KINDA_LARGE_FLOAT;
		float m_ZoomMin = 1.f;
	};

	class CameraLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::CameraPrototype& prototype) const;
		void Load(eng::CameraPrototype& prototype, eng::Visitor& visitor) const;
	};
}