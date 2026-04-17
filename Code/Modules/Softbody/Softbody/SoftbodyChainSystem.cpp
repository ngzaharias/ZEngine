#include "SoftbodyPCH.h"
#include "Softbody/SoftbodyChainSystem.h"

#include "Core/Colour.h"
#include "Core/GameTime.h"
#include "ECS/WorldView.h"
#include "ECS/QueryTypes.h"
#include "Engine/CameraHelpers.h"
#include "Engine/CameraComponent.h"
#include "Engine/InputManager.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Math/CollisionMath.h"
#include "Math/Math.h"
#include "Math/Matrix.h"
#include "Math/Plane.h"
#include "Softbody/SoftbodyChainComponent.h"
#include "Softbody/SoftbodyChainTemplate.h"

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

	Vector3f ToMouseDirection(const Vector3f& mousePosition, const eng::CameraComponent& camera, const eng::TransformComponent& transform)
	{
		if (std::holds_alternative<eng::Orthographic>(camera.m_Projection))
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

	using RemovedQuery = ecs::query
		::Condition<ecs::Alive, ecs::Dead>
		::Removed<softbody::ChainComponent>;
	if (world.HasAny<RemovedQuery>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const auto* window = windowManager.GetWindow(0);
	if (!window)
		return;

	const Vector2u& windowSize = window->GetSize();

	using UpdateQuery = ecs::query
		::Include<
		softbody::ChainComponent,
		const eng::TransformComponent,
		const softbody::ChainTemplate>;
	for (auto&& chainView : world.Query<UpdateQuery>())
	{
		using CameraQuery = ecs::query
			::Include<
			const eng::ActiveComponent,
			const eng::CameraComponent,
			const eng::TransformComponent>;
		for (auto&& cameraView : world.Query<CameraQuery>())
		{
			const auto& input = world.ReadResource<eng::InputManager>();
			const auto& cameraProjection = cameraView.ReadRequired<eng::CameraComponent>();
			const auto& cameraTransform = cameraView.ReadRequired<eng::TransformComponent>();

			const Plane3f plane = Plane3f(Vector3f::AxisZ, Vector3f::Zero);
			const Ray3f ray = eng::ScreenToRay(
				cameraProjection,
				cameraTransform,
				*window,
				input.m_MousePosition);

			Vector3f intersectPos;
			if (math::Intersection(ray, plane, intersectPos))
			{
				auto& transformComponent = chainView.WriteRequired<eng::TransformComponent>();
				transformComponent.m_Translate = intersectPos;
			}
		}

		auto& chainComponent = chainView.WriteRequired<softbody::ChainComponent>();
		const auto& chainTemplate = chainView.ReadRequired<softbody::ChainTemplate>();
		const auto& transformComponent = chainView.ReadRequired<eng::TransformComponent>();
		if (!chainComponent.m_Links.IsEmpty())
		{
			// update first node
			chainComponent.m_Links[0].m_Position = transformComponent.m_Translate.XY();
			if (chainComponent.m_Links.GetCount() >= 2)
			{
				softbody::ChainLink& linkH = chainComponent.m_Links[0];
				softbody::ChainLink& linkI = chainComponent.m_Links[1];
				if (!math::IsNearly(linkH.m_Position, linkI.m_Position))
					linkH.m_Direction = (linkH.m_Position - linkI.m_Position).Normalized();
			}

			const float radians = math::ToRadians(chainTemplate.m_Angle);
			const bool isAngleConstrained = chainTemplate.m_Angle > 0.f;
			const bool isLengthConstrained = chainTemplate.m_Radius > 0.f;
			for (int32 i = 1; i < chainComponent.m_Links.GetCount(); ++i)
			{
				softbody::ChainLink& linkH = chainComponent.m_Links[i-1];
				softbody::ChainLink& linkI = chainComponent.m_Links[i];

				linkI.m_Position += chainTemplate.m_Gravity * gameTime.m_DeltaTime;

				linkI.m_Direction = (linkH.m_Position - linkI.m_Position).Normalized();
				if (isAngleConstrained)
					linkI.m_Direction = ConstrainAngle(linkH.m_Direction, linkI.m_Direction, radians);
				if (isLengthConstrained)
					linkI.m_Position = ConstrainLength(linkH.m_Position, linkI.m_Direction, chainTemplate.m_Radius);
			}
		}
	}

	using TemplateAddedQuery = ecs::query
		::Added<const softbody::ChainTemplate>
		::Include<const softbody::ChainTemplate>;
	for (auto&& view : world.Query<TemplateAddedQuery>())
	{
		const auto& chainTemplate = view.ReadRequired<softbody::ChainTemplate>();
		auto& chainComponent = world.AddComponent<softbody::ChainComponent>(view);
		for (int32 i = 0; i < chainTemplate.m_Links; ++i)
			chainComponent.m_Links.Emplace();
	}

	using TemplateUpdatedQuery = ecs::query
		::Updated<const softbody::ChainTemplate>
		::Include<softbody::ChainComponent, const softbody::ChainTemplate>;
	for (auto&& view : world.Query<TemplateUpdatedQuery>())
	{
		const auto& chainTemplate = view.ReadRequired<softbody::ChainTemplate>();
		auto& chainComponent = view.WriteRequired<softbody::ChainComponent>();

		chainComponent.m_Links.RemoveAll();
		for (int32 i = 0; i < chainTemplate.m_Links; ++i)
			chainComponent.m_Links.Emplace();
	}

	for (auto&& view : world.Query<ecs::query::Removed<const softbody::ChainTemplate>>())
	{
		world.RemoveComponent<softbody::ChainComponent>(view);
	}
}
