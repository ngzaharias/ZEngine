#pragma once

#include "Core/Array.h"
#include "Core/Colour.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"

namespace render
{
	struct Batch
	{
		Array<Matrix4x4> m_Models;
		Array<Vector4f>  m_TexParams;
		Array<Colour>    m_Colours;
	};
}