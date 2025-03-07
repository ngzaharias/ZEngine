#include "EditorPCH.h"
#include "Editor/TextureHelpers.h"

#include "Math/Math.h"
#include "Math/Vector.h"
#include "Math/VectorMath.h"

Vector2f editor::FitImageToRegion(const Vector2f& imageSize, const Vector2f& regionSize)
{
	const Vector2f ratios = math::Divide(regionSize, imageSize);
	const float ratio = math::Min(ratios.x, ratios.y);
	return imageSize * ratio;
}