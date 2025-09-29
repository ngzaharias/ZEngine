#include "SoftbodyPCH.h"
#include "Softbody/SoftbodyChainSystem.h"

#include "Core/Colour.h"
#include "Core/GameTime.h"
#include "ECS/QueryTypes.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Math/CollisionMath.h"
#include "Math/Math.h"
#include "Math/Matrix.h"
#include "Math/Plane.h"
#include "Softbody/SoftbodyComponents.h"

namespace
{
	const str::Name strInput = str::Name::Create("SoftbodyChain");
	const str::Name strSelect = str::Name::Create("SoftbodyChain_Select");

	Vector2f ConstrainAngle(const Vector2f& directionA, const Vector2f& directionB, const float constraint)
	{
		const float angleH = math::AngleSigned(directionA, Vector2f::AxisY);
		const float angleI = math::AngleSigned(directionB, Vector2f::AxisY);
		const float diff = math::AngleSigned(directionA, -directionB);
		if (math::Absolute(diff) < constraint)
		{
			const float angle = diff > 0.f
				? angleH - constraint + PI_ONE
				: angleH + constraint + PI_ONE;
			return Vector2f::AxisY * Quaternion::FromAxisAngle(Vector3f::AxisZ, -angle);
		}
		return directionB;
	}

	Vector2f ConstrainLength(const Vector2f& positionA, const Vector2f& directionB, const float constraint)
	{
		return positionA - directionB * constraint;
	}

	Vector3f ToMouseDirection(const Vector3f& mousePosition, const eng::camera::ProjectionComponent& camera, const eng::TransformComponent& transform)
	{
		if (std::holds_alternative<eng::camera::Orthographic>(camera.m_Projection))
		{
			const Quaternion cameraRotate = Quaternion::FromRotator(transform.m_Rotate);
			return Vector3f::AxisZ * cameraRotate;
		}

		return (mousePosition - transform.m_Translate).Normalized();
	}
}

void softbody::ChainSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<ecs::query::Added<softbody::ChainComponent>>())
	{
		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::Gameplay;
		layer.m_Bindings.Emplace(strSelect, input::EKey::Mouse_Left);

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInput, layer);
	}

	if (world.HasAny<ecs::query::Removed<softbody::ChainComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const auto* window = windowManager.GetWindow(0);
	if (!window)
		return;

	const Vector2u& windowSize = window->GetSize();
	for (const ecs::Entity& chainEntity : world.Query<ecs::query::Include<const eng::TransformComponent, const softbody::ChainComponent>>())
	{
		for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::ProjectionComponent, const eng::TransformComponent>>())
		{
			const auto& input = world.ReadResource<eng::InputManager>();
			const auto& cameraProjection = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
			const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

			const Plane3f plane = Plane3f(Vector3f::AxisZ, Vector3f::Zero);
			const Ray3f ray = eng::camera::ScreenToRay(
				cameraProjection,
				cameraTransform,
				*window,
				input.m_MousePosition);

			Vector3f intersectPos;
			if (math::Intersection(ray, plane, intersectPos))
			{
				auto& chainTransform = world.WriteComponent<eng::TransformComponent>(chainEntity);
				chainTransform.m_Translate = intersectPos;
			}
		}

		auto& chain = world.WriteComponent<softbody::ChainComponent>(chainEntity);
		const auto& transform = world.ReadComponent<eng::TransformComponent>(chainEntity);
		if (!chain.m_Links.IsEmpty())
		{
			// update first node
			chain.m_Links[0].m_Position = transform.m_Translate.XY();
			if (chain.m_Links.GetCount() >= 2)
			{
				softbody::Link& linkH = chain.m_Links[0];
				softbody::Link& linkI = chain.m_Links[1];
				if (!math::IsNearly(linkH.m_Position, linkI.m_Position))
					linkH.m_Direction = (linkH.m_Position - linkI.m_Position).Normalized();
			}

			const float radians = math::ToRadians(chain.m_Angle);
			const bool isAngleConstrained = chain.m_Angle > 0.f;
			const bool isLengthConstrained = chain.m_Radius > 0.f;
			for (int32 i = 1; i < chain.m_Links.GetCount(); ++i)
			{
				softbody::Link& linkH = chain.m_Links[i-1];
				softbody::Link& linkI = chain.m_Links[i];

				linkI.m_Position += chain.m_Gravity * gameTime.m_DeltaTime;

				linkI.m_Direction = (linkH.m_Position - linkI.m_Position).Normalized();
				if (isAngleConstrained)
					linkI.m_Direction = ConstrainAngle(linkH.m_Direction, linkI.m_Direction, radians);
				if (isLengthConstrained)
					linkI.m_Position = ConstrainLength(linkH.m_Position, linkI.m_Direction, chain.m_Radius);
			}
		}
	}
}
