#include "GameDebugPCH.h"
#include "GameDebug/PhysicsSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LinesComponent.h"
#include "Engine/RigidDynamicComponent.h"
#include "Engine/RigidStaticComponent.h"
#include "Engine/SettingsComponents.h"
#include "Math/AABB.h"
#include "Math/Matrix.h"
#include "Math/Sphere.h"
#include "Math/Vector.h"
#include "Math/Quaternion.h"

#include <PhysX/PxRigidDynamic.h>
#include <PhysX/PxRigidStatic.h>
#include <PhysX/PxPhysics.h>
#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/PxScene.h>

namespace
{
	constexpr Vector4f s_ColourM = Vector4f(1.f, 0.f, 1.f, 1.f);

	Vector3f ToVector3f(const physx::PxVec3& value)
	{
		return Vector3f(value.x, value.y, value.z);
	}

	Quaternion ToQuaternion(const physx::PxQuat& value)
	{
		return Quaternion(value.x, value.y, value.z, value.w);
	}

	Matrix4x4 ToTransform(const physx::PxTransform& value)
	{
		return Matrix4x4::FromTransform(
			ToVector3f(value.p),
			ToQuaternion(value.q),
			Vector3f::One);
	}

	using World = dbg::PhysicsSystem::World;
	void RenderShape(World& world, const physx::PxActor& actor, const physx::PxShape& shape, const physx::PxTransform& actorTransform)
	{
		auto& linesComponent = world.WriteSingleton<eng::LinesComponent>();

		const physx::PxTransform& shapeTransform = shape.getLocalPose();
		const Matrix4x4 transform = ToTransform(shapeTransform) * ToTransform(actorTransform);
		const Vector3f translate = Vector3f(transform[3][0], transform[3][1], transform[3][2]);

		switch (shape.getGeometryType())
		{
		case physx::PxGeometryType::eBOX:
		{
			physx::PxBoxGeometry boxGeo;
			shape.getBoxGeometry(boxGeo);

			const Vector3f extents = ToVector3f(boxGeo.halfExtents);
			const AABB3f aabb = AABB3f::FromExtents(extents);
			linesComponent.AddAABB(translate, aabb, s_ColourM);
		} break;
		case physx::PxGeometryType::eSPHERE:
		{
			physx::PxSphereGeometry sphereGeo;
			shape.getSphereGeometry(sphereGeo);

			const Sphere3f sphere = Sphere3f(translate, sphereGeo.radius);
			linesComponent.AddSphere(translate, sphere, s_ColourM);
			
		} break;

		// #todo:
		}
	}
}

void dbg::PhysicsSystem::Update(World& world, const GameTime& gameTime)
{
	const auto& settings = world.ReadSingleton<const eng::settings::DebugComponent>();
	if (!settings.m_ArePhysicsEnabled)
		return;

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::RigidStaticComponent>>())
	{
		const auto& component = world.ReadComponent<eng::RigidStaticComponent>(entity);
		for (const physx::PxShape* shape : component.m_Shapes)
			RenderShape(world, *component.m_Actor, *shape, component.m_Actor->getGlobalPose());
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::RigidDynamicComponent>>())
	{
		const auto& component = world.ReadComponent<eng::RigidDynamicComponent>(entity);
		for (const physx::PxShape* shape : component.m_Shapes)
			RenderShape(world, *component.m_Actor, *shape, component.m_Actor->getGlobalPose());
	}
}
