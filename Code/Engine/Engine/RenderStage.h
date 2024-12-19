#pragma once

#include "Core/Array.h"
#include "Core/Colour.h"
#include "Core/Guid.h"
#include "ECS/Entity.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"

//#define ASSERT_RENDER

namespace ecs
{
	class EntityWorld;
}

namespace eng
{
	struct RenderBatchID
	{
		ecs::Entity m_Entity = { };

		float m_Depth = 0.f;
		str::Guid m_TextureId = {};
		str::Guid m_ShaderId = {};
		str::Guid m_StaticMeshId = {};
	};

	struct RenderBatchData
	{
		Array<Matrix4x4> m_Models;
		Array<Vector4f>  m_TexParams;
		Array<Colour>    m_Colours;
	};

	struct RenderStageData
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

	class RenderStage
	{
	public:
		virtual ~RenderStage() { }

		virtual void Initialise(ecs::EntityWorld& world) { }
		virtual void Shutdown(ecs::EntityWorld& world) { }

		virtual void Render(ecs::EntityWorld& world) = 0;
	};
}