#pragma once

#include "Core/Variant.h"
#include "Math/Math.h"
#include "Math/Vector.h"

namespace eng::camera
{
	enum class EAxis
	{
		Horizontal = 0,
		Vertical,
	};

	struct Bound2D
	{
		Vector2f m_Max = Vector2f(+KINDA_LARGE_FLOAT);
		Vector2f m_Min = Vector2f(-KINDA_LARGE_FLOAT);
	};

	struct Move2D {};
	struct Move3D {};

	struct Zoom2D
	{
		float m_Max = KINDA_LARGE_FLOAT;
		float m_Min = 1.f;
	};

	using BehaviourBound = Variant<Bound2D>;
	using BehaviourMove = Variant<Move2D, Move3D>;
	using BehaviourZoom = Variant<Zoom2D>;

	struct Cinematic
	{
		float m_ClippingNear = 30.f;
		float m_ClippingFar = 100000.f;
	};

	struct Orthographic
	{
		float m_ClippingNear = 30.f;
		float m_ClippingFar = 100000.f;
		float m_Size = 1000.f;
	};

	struct Perspective
	{
		float m_ClippingNear = 30.f;
		float m_ClippingFar = 100000.f;
		float m_FieldOfView = 60.f;
		EAxis m_FoVAxis = EAxis::Horizontal;
	};

	using Projection = Variant<
		//Cinematic, 
		Orthographic, 
		Perspective>;
}