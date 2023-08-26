#include "EnginePCH.h"
#include "Engine/Texture2DAsset.h"

#include "Engine/AssetManager.h"
#include "Engine/Visitor.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace
{
	const str::Name strSourceFile = NAME("m_SourceFile");
}

bool eng::Texture2DAssetLoader::Load(Texture2DAsset* asset, eng::Visitor& visitor) const
{
	visitor.Visit(strSourceFile, asset->m_SourceFile, {});

	// #todo: error message
	// #todo: fallback to default asset
	if (asset->m_SourceFile.IsEmpty())
		return false;

	const str::Path filepath = str::Path(str::EPath::Assets, asset->m_SourceFile);

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filepath.ToChar(), &asset->m_Width, &asset->m_Height, &asset->m_Channels, STBI_rgb_alpha);
	if (!data)
	{
		// #todo: fallback to default asset
		return false;
	}

	glGenTextures(1, &asset->m_TextureId);
	glBindTexture(GL_TEXTURE_2D, asset->m_TextureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, asset->m_Width, asset->m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	return true;
}
