#pragma once

#include "Core/Array.h"
#include "Core/Colour.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"

namespace render
{
	struct Data
	{
		const Matrix4x4& m_CameraProj;
		const Matrix4x4& m_CameraView;

		Array<Vector3f> m_LightAmbient_Colour;

		Array<Vector3f> m_LightDirectional_Colour;
		Array<Vector3f> m_LightDirectional_Direction;

		Array<float> 	m_LightPoint_Range;
		Array<Vector3f> m_LightPoint_Colour;
		Array<Vector3f> m_LightPoint_Position;
	};
}