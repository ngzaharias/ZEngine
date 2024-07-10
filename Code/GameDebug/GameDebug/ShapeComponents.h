#pragma once

#include "Core/Variant.h"
#include "ECS/Component.h"
#include "Math/AABB.h"
#include "Math/Circle.h"
#include "Math/Line.h"
#include "Math/Ray.h"
#include "Math/Segment.h"
#include "Math/Triangle.h"

namespace dbg
{
	using Shape = Variant<
		AABB2f,
		Circle2f,
		Line2f,
		Ray2f,
		Segment2f,
		Triangle2f>;

	struct ShapeWindowComponent : public ecs::Component<ShapeWindowComponent>
	{
		str::String m_DockspaceLabel = {};
		str::String m_InspectorLabel = {};
		str::String m_PlottingLabel = {};

		Shape m_ShapeA = AABB2f(-Vector2f::One, +Vector2f::One);
		Shape m_ShapeB = Circle2f(Vector2f::Zero, 1.f);
	};
}