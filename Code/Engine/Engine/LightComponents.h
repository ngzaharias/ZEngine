#pragma once

#include <Core/Variant.h>
#include <Core/Vector.h>

#include <ECS/Component.h>

namespace eng
{
	struct LightAmbientComponent final : public ecs::Component<LightAmbientComponent>
	{
		Vector3f m_Colour = Vector3f::One;
	};

	struct LightDirectionalComponent final : public ecs::Component<LightDirectionalComponent>
	{
		Vector3f m_Colour = Vector3f::One;
	};

	struct LightPointComponent final : public ecs::Component<LightPointComponent>
	{
		float m_Range = 500.f;
		Vector3f m_Colour = Vector3f::One;
	};
}