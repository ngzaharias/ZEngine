#include "GameDebugPCH.h"
#include "GameDebug/SplineWindowComponent.h"

#include "imgui/Inspector.h"

template<>
bool imgui::Inspector::WriteCustom(debug::BezierQuadratic& value)
{
	bool result = false;
	result |= Write("Control", value.c);
	result |= Write("1: Point", value.p1);
	result |= Write("2: Point", value.p2);
	return result;
}

template<>
bool imgui::Inspector::WriteCustom(debug::BezierCubic& value)
{
	bool result = false;
	result |= Write("1: Point", value.p1);
	result |= Write("1: Control", value.c1);
	result |= Write("2: Control", value.c2);
	result |= Write("2: Point", value.p2);
	return result;
}

template<>
bool imgui::Inspector::WriteCustom(debug::CatmullRom& value)
{
	bool result = false;
	result |= Write("1: Control", value.c1);
	result |= Write("1: Point", value.p1);
	result |= Write("2: Point", value.p2);
	result |= Write("2: Control", value.c2);
	return result;
}

template<>
bool imgui::Inspector::WriteCustom(debug::Hermite& value)
{
	bool result = false;
	result |= Write("Velocity 2", value.v2);
	result |= Write("1: Point", value.p1);
	result |= Write("1: Velocity", value.v1);
	result |= Write("2: Point", value.p2);
	result |= Write("2: Velocity", value.v2);
	return result;
}