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

	struct UserInterface
	{
		float m_ClippingNear = 0.3f;
		float m_ClippingFar = 100.f;
	};

	using Projection = Variant<
		//Cinematic, 
		Orthographic,
		Perspective,
		UserInterface>;
}