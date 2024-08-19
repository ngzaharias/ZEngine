#pragma once

#include "Core/Variant.h"

namespace camera
{
	enum class EAxis
	{
		Horizontal = 0,
		Vertical,
	};

	enum class EBehaviour
	{
		Static = 0,
		Free2D,
		Free3D,
	};

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