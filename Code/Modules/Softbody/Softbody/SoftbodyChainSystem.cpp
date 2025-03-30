#include "SoftbodyPCH.h"
#include "Softbody/SoftbodyChainSystem.h"

#include "Core/Colour.h"
#include "Core/GameTime.h"
#include "ECS/QueryTypes.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/LinesComponent.h"
#include "Engine/InputManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Math/Circle.h"
#include "Math/CollisionMath.h"
#include "Math/Plane.h"
#include "Math/Quaternion.h"
#include "Math/Ray.h"
#include "Softbody/SoftbodyComponents.h"

namespace
{
	const str::Name strInput = str::Name::Create("SoftbodyChain");
	const str::Name strSelect = str::Name::Create("SoftbodyChain_Select");

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

	const Vector2u& resolution = window->GetResolution();
	for (const ecs::Entity& chainEntity : world.Query<ecs::query::Include<const eng::TransformComponent, const softbody::ChainComponent>>())
	{
		for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::ProjectionComponent, const eng::TransformComponent>>())
		{
			const auto& input = world.ReadResource<eng::InputManager>();
			const auto& projection = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
			const auto& transform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

			const Vector3f mousePosition = eng::camera::ScreenToWorld(input.m_MousePosition, projection.m_Projection, transform.ToTransform(), resolution);
			const Vector3f mouseDirection = ToMouseDirection(mousePosition, projection, transform);

			Plane3f plane;
			plane.m_Normal = Vector3f::AxisZ;
			plane.m_Point = Vector3f::Zero;

			Ray3f ray;
			ray.m_Position = mousePosition;
			ray.m_Direction = mouseDirection;

			Vector3f intersectPos;
			if (math::Intersection(ray, plane, intersectPos))
			{
				auto& chainTransform = world.WriteComponent<eng::TransformComponent>(chainEntity);
				chainTransform.m_Translate = intersectPos;
			}
		}

		auto& lines = world.WriteSingleton<eng::LinesComponent>();
		auto& chain = world.WriteComponent<softbody::ChainComponent>(chainEntity);
		const auto& transform = world.ReadComponent<eng::TransformComponent>(chainEntity);
		if (!chain.m_Links.IsEmpty())
		{
			chain.m_Links[0].m_Position = transform.m_Translate;
			for (int32 i = 1; i < chain.m_Links.GetCount(); ++i)
			{
				auto& linkH = chain.m_Links[i - 1];
				auto& linkI = chain.m_Links[i];

				const Vector3f direction = (linkI.m_Position - linkH.m_Position).Normalized();
				linkI.m_Position = linkH.m_Position + direction * linkH.m_Radius;
			}
		}

		for (const softbody::Link& link : chain.m_Links)
			lines.AddCircle(link.m_Position, Circle2f(50.f), 30.f, Colour::White);
	}
}
