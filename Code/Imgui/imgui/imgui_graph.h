#pragma once

#include "Math/Vector.h"

class Circle2f;
class Line2f;
class Ray2f;
class AABB2f;
class Segment2f;
class Triangle2f;
typedef int ImDrawFlags;

enum ImGuiGraphFlags_
{
	ImGuiGraphFlags_Grid     = 1 << 0,
	ImGuiGraphFlags_TextX    = 1 << 1,
	ImGuiGraphFlags_TextY    = 1 << 2,
};
using ImGuiGraphFlags = int; // -> enum ImGuiGraphFlags_

namespace imgui
{
	bool BeginGraph(const char* label, Vector2f graph_size = Vector2f::Zero, ImGuiGraphFlags flags = 0);
	void EndGraph();

	void GraphShape(Circle2f& value, int segments = 0, float thickness = 1.f);
	void GraphShape(Line2f& value, float thickness = 1.f);
	void GraphShape(Ray2f& value, float thickness = 1.f);
	void GraphShape(AABB2f& value, float rounding = 0.f, float thickness = 1.f, ImDrawFlags flags = 0);
	void GraphShape(Segment2f& value, float thickness = 1.f);
	void GraphShape(Triangle2f& value, float thickness = 1.f);
	void GraphShape(Vector2f& value, float thickness = 1.f);
}