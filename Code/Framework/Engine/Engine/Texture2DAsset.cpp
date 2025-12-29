#include "EnginePCH.h"
#include "Engine/Texture2DAsset.h"

#include "Core/Thread.h"
#include "Engine/AssetManager.h"
#include "Engine/FileHelpers.h"
#include "Engine/Visitor.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace
{
	const str::Name strSourceFile = NAME("m_SourceFile");

	bool LoadImage(eng::Texture2DAsset& asset, const str::Path& filepath)
	{
		PROFILE_FUNCTION();

		stbi_set_flip_vertically_on_load(true);
		asset.m_TextureData = stbi_load(filepath.ToChar(), &asset.m_Width, &asset.m_Height, &asset.m_Channels, STBI_rgb_alpha);
		if (!asset.m_TextureData)
		{
			const char* failureReason = stbi_failure_reason();
			Z_LOG(ELog::Assert, "Failed to load Texture2D! Reason: {} \nPath: {}", filepath.ToChar(), failureReason);
			return false;
		}

		return true;
	}
}

void eng::Texture2DAssetLoader::Bind(eng::Texture2DAsset& asset) const
{
	PROFILE_FUNCTION();
	Z_PANIC(core::IsMainThread(), "OpenGL functions must be called from the main thread!");

	glGenTextures(1, &asset.m_TextureId);
	glBindTexture(GL_TEXTURE_2D, asset.m_TextureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, asset.m_Width, asset.m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, asset.m_TextureData);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void eng::Texture2DAssetLoader::Unbind(eng::Texture2DAsset& asset) const
{
	PROFILE_FUNCTION();
	Z_PANIC(core::IsMainThread(), "OpenGL functions must be called from the main thread!");

	glDeleteTextures(1, &asset.m_TextureId);
}

bool eng::Texture2DAssetLoader::Import(eng::Texture2DAsset& asset, const str::Path& filepath) const
{
	PROFILE_FUNCTION();

	// #hack: no good way to do relative paths
	str::Path rootDirectory = eng::GetCurrentFilepath();
	rootDirectory = rootDirectory.GetParent();
	rootDirectory = rootDirectory.GetParent();
	rootDirectory = rootDirectory.GetParent();
	rootDirectory = rootDirectory.GetParent();

	str::String relativePath = str::String(filepath);
	str::TrimLeft(relativePath, rootDirectory);
	relativePath = ".." + relativePath;
	asset.m_SourceFile = relativePath;

	return LoadImage(asset, filepath);
}

bool eng::Texture2DAssetLoader::Load(eng::Texture2DAsset& asset, eng::Visitor& visitor) const
{
	PROFILE_FUNCTION();

	visitor.Read(strSourceFile, asset.m_SourceFile, {});

	// #todo: error message
	// #todo: fallback to default asset
	if (asset.m_SourceFile.IsEmpty())
		return false;

	// #todo: fallback to default asset on failure
	const str::Path filepath = str::Path(str::EPath::Assets, asset.m_SourceFile);
	return LoadImage(asset, filepath);
}

bool eng::Texture2DAssetLoader::Save(eng::Texture2DAsset& asset, eng::Visitor& visitor) const
{
	PROFILE_FUNCTION();
	visitor.Write(strSourceFile, asset.m_SourceFile);
	return true;
}

bool eng::Texture2DAssetLoader::Unload(eng::Texture2DAsset& asset) const
{
	PROFILE_FUNCTION();
	stbi_image_free(asset.m_TextureData);
	return true;
}
