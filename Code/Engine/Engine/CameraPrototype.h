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
}

namespace eng::camera
{
	struct Prototype final
	{
		eng::camera::Projection m_Projection = eng::camera::Perspective();
		eng::camera::EBehaviour m_Behaviour = eng::camera::EBehaviour::Free3D;
		Vector3f m_FrustrumEdgeMax = Vector3f(+KINDA_LARGE_FLOAT);
		Vector3f m_FrustrumEdgeMin = Vector3f(-KINDA_LARGE_FLOAT);
		float m_ZoomMax = KINDA_LARGE_FLOAT;
		float m_ZoomMin = 1.f;
	};

	class Loader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const Prototype& prototype) const;
		void Load(Prototype& prototype, eng::Visitor& visitor) const;
	};
}