#include "EnginePCH.h"
#include "Engine/TransformComponent.h"

#include "Math/Matrix.h"

Matrix4x4 eng::TransformComponent::ToTransform() const
{
	return Matrix4x4::FromTransform(m_Translate, m_Rotate, m_Scale);
}
