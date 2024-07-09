#pragma once

#include "Core/Array.h"
#include "Core/Vector.h"
#include "ECS/Component.h"

namespace eng
{
	struct DynamicMeshBinding
	{
		uint32 m_AttributeObject = 0;

		// attributes
		uint32 m_VertexBuffer = 0;
		uint32 m_TexCoordBuffer = 0;
		uint32 m_NormalBuffer = 0;
		uint32 m_IndexBuffer = 0;
	};

	struct DynamicMeshComponent : public ecs::Component<DynamicMeshComponent>
	{
		Array<uint32> m_Indices;
		Array<Vector3f> m_Normals;
		Array<Vector2f> m_TexCoords;
		Array<Vector3f> m_Vertices;

		DynamicMeshBinding m_Binding;
	};
}