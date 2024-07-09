#pragma once

#include "Core/AABB.h"
#include "Core/Circle.h"
#include "Core/Line.h"
#include "Core/Ray.h"
#include "Core/Segment.h"
#include "Core/Triangle.h"
#include "Core/Variant.h"
#include "ECS/Component.h"

namespace dbg
{
	using Shape = Variant<
		Circle2f,
		Line2f,
		Ray2f,
		AABB2f,
		Segment2f,
		Triangle2f>;

	struct ShapeWindowComponent : public ecs::Component<ShapeWindowComponent>
	{
		str::String m_DockspaceLabel = {};
		str::String m_InspectorLabel = {};
		str::String m_PlottingLabel = {};

		Shape m_ShapeA = {};
		Shape m_ShapeB = {};
	};
}