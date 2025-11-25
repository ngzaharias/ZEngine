#include "GameDebugPCH.h"
#include "GameDebug/PhysicsSystem.h"

#include "Core/Colour.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LinesComponent.h"
#include "Engine/PhysicsComponent.h"
#include "Engine/RigidDynamicComponent.h"
#include "Engine/RigidStaticComponent.h"
#include "Engine/SettingsComponents.h"
#include "Math/AABB.h"
#include "Math/Matrix.h"
#include "Math/OBB.h"
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
		const Rotator rotate = Rotator::FromMatrix(transform);

		switch (shape.getGeometryType())
		{
		case physx::PxGeometryType::eBOX:
		{
			physx::PxBoxGeometry boxGeo;
			shape.getBoxGeometry(boxGeo);

			const Vector3f extents = ToVector3f(boxGeo.halfExtents);
			const OBB3f obb = OBB3f::FromExtents(Vector3f::Zero, rotate, extents);
			linesComponent.AddOBB(translate, obb, Colour::Magenta);
		} break;
		case physx::PxGeometryType::eSPHERE:
		{
			physx::PxSphereGeometry sphereGeo;
			shape.getSphereGeometry(sphereGeo);

			const Sphere3f sphere = Sphere3f(Vector3f::Zero, sphereGeo.radius);
			linesComponent.AddIcosphere(translate, sphere, Colour::Magenta);
			
		} break;

		// #todo:
		}
	}
}

void dbg::PhysicsSystem::Update(World& world, const GameTime& gameTime)
{
	const auto& settings = world.ReadSingleton<eng::settings::DebugComponent>();
	if (!settings.m_ArePhysicsEnabled)
		return;

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::PhysicsComponent>>())
	{
		const auto& component = world.ReadComponent<eng::PhysicsComponent>(entity);
		for (const physx::PxShape* shape : component.m_PxShapes)
			RenderShape(world, *component.m_PxRigidActor, *shape, component.m_PxRigidActor->getGlobalPose());
	}

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
