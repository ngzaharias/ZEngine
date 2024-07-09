#pragma once

#include "Core/AABB.h"
#include "Core/Array.h"
#include "Core/Types.h"
#include "Core/Vector.h"
#include "Engine/Asset.h"
#include "Engine/AssetLoader.h"

namespace eng
{
	class Visitor;
}

namespace eng
{
	struct StaticMeshBinding
	{
		uint32 m_AttributeObject = 0;

		// attributes
		uint32 m_VertexBuffer = 0;
		uint32 m_TexCoordBuffer = 0;
		uint32 m_NormalBuffer = 0;
		uint32 m_IndexBuffer = 0;
	};

	struct StaticMeshAsset : public eng::Asset
	{
		str::Path m_SourceFile = { };

		AABB3f m_Bounds;

		Array<uint32> m_Indices;
		Array<Vector3f> m_Normals;
		Array<Vector2f> m_TexCoords0;
		Array<Vector2f> m_TexCoords1;
		Array<Vector2f> m_TexCoords2;
		Array<Vector2f> m_TexCoords3;
		Array<Vector3f> m_Vertices;

		StaticMeshBinding m_Binding;
	};

	class StaticMeshAssetLoader final : public eng::AssetLoader
	{
	public:
		static void Bind(eng::StaticMeshAsset& asset);

		bool Load(StaticMeshAsset& asset, eng::Visitor& visitor) const;
	};
}