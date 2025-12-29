#include "EnginePCH.h"
#include "Engine/StaticMeshAsset.h"

#include "Core/Algorithms.h"
#include "Core/Thread.h"
#include "Engine/AssetManager.h"
#include "Engine/Visitor.h"

#include <assimp/BaseImporter.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	const str::Name strSourceFile = NAME("m_SourceFile");
}

bool eng::StaticMeshAssetLoader::Bind(eng::StaticMeshAsset& asset) const
{
	PROFILE_FUNCTION();
	Z_PANIC(core::IsMainThread(), "OpenGL functions must be called from the main thread!");
	
	auto& binding = asset.m_Binding;
	glGenVertexArrays(1, &binding.m_AttributeObject);
	glBindVertexArray(binding.m_AttributeObject);

	// vertex
	glGenBuffers(1, &binding.m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, binding.m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * asset.m_Vertices.GetCount(), asset.m_Vertices.GetData(), GL_STATIC_DRAW);

	// texture coordinates
	glGenBuffers(1, &binding.m_TexCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, binding.m_TexCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f) * asset.m_TexCoords0.GetCount(), asset.m_TexCoords0.GetData(), GL_STATIC_DRAW);

	// normal
	glGenBuffers(1, &binding.m_NormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, binding.m_NormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * asset.m_Normals.GetCount(), asset.m_Normals.GetData(), GL_STATIC_DRAW);

	// index
	glGenBuffers(1, &binding.m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, binding.m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * asset.m_Indices.GetCount(), asset.m_Indices.GetData(), GL_STATIC_DRAW);

	// unbind when we're done to avoid accidentally writing to these buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

bool eng::StaticMeshAssetLoader::Unbind(eng::StaticMeshAsset& asset) const
{
	PROFILE_FUNCTION();
	Z_PANIC(core::IsMainThread(), "OpenGL functions must be called from the main thread!");

	auto& binding = asset.m_Binding;
	glDeleteVertexArrays(1, &binding.m_AttributeObject);
	return true;
}

bool eng::StaticMeshAssetLoader::Load(eng::StaticMeshAsset& asset, eng::Visitor& visitor) const
{
	PROFILE_FUNCTION();

	visitor.Read(strSourceFile, asset.m_SourceFile, {});

	// #todo: error message
	// #todo: fallback to default asset
	if (asset.m_SourceFile.IsEmpty())
		return false;

	const str::Path filepath = str::Path(str::EPath::Assets, asset.m_SourceFile);
	const uint32 flags =
		aiProcess_CalcTangentSpace |
		aiProcess_FlipWindingOrder |
		aiProcess_GenBoundingBoxes |
		aiProcess_GlobalScale |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_Triangulate;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath.ToChar(), flags);

	Z_PANIC(scene, "");
	Z_PANIC(scene->HasMeshes(), "");

	for (uint32 i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];

		// Bounds
		{
			asset.m_Bounds.m_Min.x = mesh->mAABB.mMin.x;
			asset.m_Bounds.m_Min.y = mesh->mAABB.mMin.y;
			asset.m_Bounds.m_Min.z = mesh->mAABB.mMin.z;
			asset.m_Bounds.m_Max.x = mesh->mAABB.mMax.x;
			asset.m_Bounds.m_Max.y = mesh->mAABB.mMax.y;
			asset.m_Bounds.m_Max.z = mesh->mAABB.mMax.z;
		}

		// Indices
		{
			aiFace* face = mesh->mFaces;
			const int32 count = mesh->mNumFaces;

			asset.m_Indices.Reserve(count * 3);
			for (int32 j = 0; j < count; ++j, ++face)
			{
				uint32* index = face->mIndices;
				const int32 indices = face->mNumIndices;
				for (int32 k = 0; k < indices; ++k, ++index)
					asset.m_Indices.Append(*index);
			}
		}

		// Normals
		{
			aiVector3D* normal = mesh->mNormals;
			const int32 count = mesh->HasNormals() ? mesh->mNumVertices : 0;

			asset.m_Normals.Reserve(count);
			for (int32 j = 0; j < count; ++j, ++normal)
				asset.m_Normals.Emplace(normal->x, normal->z, normal->y);
		}

		// Vertices
		{
			aiVector3D* vertex = mesh->mVertices;
			const int32 count = mesh->mNumVertices;

			asset.m_Vertices.Reserve(mesh->mNumVertices);
			for (int32 j = 0; j < count; ++j, ++vertex)
				asset.m_Vertices.Emplace(vertex->x, vertex->z, vertex->y);
		}

		// Texture Coordinates 0
		{
			aiVector3D* coord = mesh->mTextureCoords[0];
			const int32 count = mesh->HasTextureCoords(0) ? mesh->mNumVertices : 0;

			asset.m_TexCoords0.Reserve(count);
			for (int32 j = 0; j < count; ++j, ++coord)
				asset.m_TexCoords0.Emplace(coord->x, coord->y);
		}

		// Texture Coordinates 1
		{
			aiVector3D* coord = mesh->mTextureCoords[1];
			const int32 count = mesh->HasTextureCoords(1) ? mesh->mNumVertices : 0;

			asset.m_TexCoords1.Reserve(count);
			for (int32 j = 0; j < count; ++j, ++coord)
				asset.m_TexCoords1.Emplace(coord->x, coord->y);
		}

		// Texture Coordinates 2
		{
			aiVector3D* coord = mesh->mTextureCoords[2];
			const int32 count = mesh->HasTextureCoords(2) ? mesh->mNumVertices : 0;

			asset.m_TexCoords2.Reserve(count);
			for (int32 j = 0; j < count; ++j, ++coord)
				asset.m_TexCoords2.Emplace(coord->x, coord->y);
		}

		// Texture Coordinates 3
		{
			aiVector3D* coord = mesh->mTextureCoords[3];
			const int32 count = mesh->HasTextureCoords(3) ? mesh->mNumVertices : 0;

			asset.m_TexCoords3.Reserve(count);
			for (int32 j = 0; j < count; ++j, ++coord)
				asset.m_TexCoords3.Emplace(coord->x, coord->y);
		}
	}
	return true;
}