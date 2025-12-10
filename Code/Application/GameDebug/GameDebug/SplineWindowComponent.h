#pragma once

#include "Core/String.h"
#include "Core/Variant.h"
#include "ECS/Component.h"
#include "Math/AABB.h"
#include "Math/Circle.h"
#include "Math/Line.h"
#include "Math/Ray.h"
#include "Math/Segment.h"
#include "Math/Triangle.h"

namespace debug
{
	struct BezierQuadratic
	{
		Vector2f c = Vector2f(0.f, 1.f);
		Vector2f p1 = Vector2f(-1.f, 0.f);
		Vector2f p2 = Vector2f(+1.f, 0.f);
	};

	struct BezierCubic
	{
		Vector2f c1 = Vector2f(-1.f, 1.f);
		Vector2f c2 = Vector2f(+1.f, 1.f);
		Vector2f p1 = Vector2f(-1.f, 0.f);
		Vector2f p2 = Vector2f(+1.f, 0.f);
	};

	struct CatmullRom
	{
		Vector2f c1 = Vector2f(-1.f, 0.f);
		Vector2f c2 = Vector2f(+1.f, 0.f);
		Vector2f p1 = Vector2f(-1.f, 1.f);
		Vector2f p2 = Vector2f(+1.f, 1.f);
	};

	struct Hermite
	{
		Vector2f p1 = Vector2f(-1.f, 1.f);
		Vector2f p2 = Vector2f(+1.f, 1.f);
		Vector2f v1 = Vector2f(-1.f, 0.f);
		Vector2f v2 = Vector2f(+1.f, 0.f);
	};

	using Spline = Variant<
		BezierQuadratic,
		BezierCubic,
		CatmullRom,
		Hermite>;

	struct SplineWindowComponent final : public ecs::Component<SplineWindowComponent>
	{
		int32 m_Identifier = 0;
		str::String m_DockspaceLabel = {};
		str::String m_InspectorLabel = {};
		str::String m_PlottingLabel = {};

		Spline m_Spline = BezierQuadratic();
	};
}