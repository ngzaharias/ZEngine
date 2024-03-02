#include "EnginePCH.h"
#include "Engine/FontAsset.h"

#include "Engine/AssetManager.h"
#include "Engine/TomlHelpers.h"
#include "Engine/Visitor.h"

#include <Core/Algorithms.h>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H

// https://medium.com/@calebfaith/implementing-msdf-font-in-opengl-ea09a9ab7e00
// https://freetype.org/freetype2/docs/tutorial/step1.html
// https://freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_glyphslotrec
// https://freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_get_first_char
// https://freetype.org/freetype2/docs/reference/ft2-basic_types.html#ft_pos

namespace
{
	const str::Name strAdvanceX = NAME("m_AdvanceX");
	const str::Name strAdvanceY = NAME("m_AdvanceY");
	const str::Name strCharacter = NAME("m_Character");
	const str::Name strCharacters = NAME("m_Characters");
	const str::Name strDepth = NAME("m_Depth");
	const str::Name strGlyphs = NAME("m_Glyphs");
	const str::Name strHeight = NAME("m_Height");
	const str::Name strPixelRange = NAME("m_PixelRange");
	const str::Name strSourceFile = NAME("m_SourceFile");
	const str::Name strWidth = NAME("m_Width");

	struct Character
	{
		uint32 m_AdvanceX = 0;
		uint32 m_AdvanceY = 0;
		str::String m_Character = { };
		str::Path m_SourceFile = { };
	};
}

template<>
void eng::Visitor::VisitCustom<::Character>(::Character& value)
{
	SetInline();
	Visit(strAdvanceX, value.m_AdvanceX, 200);
	Visit(strAdvanceY, value.m_AdvanceY, 0);
	Visit(strCharacter, value.m_Character, {});
	Visit(strSourceFile, value.m_SourceFile, {});
}

bool eng::FontAssetLoader::Save(FontAsset* asset, eng::Visitor& visitor) const
{
	visitor.Visit(strWidth, asset->m_Width, 0);
	visitor.Visit(strHeight, asset->m_Height, 0);
	visitor.Visit(strPixelRange, asset->m_PixelRange, 0);

	if (!asset->m_Glyphs.IsEmpty())
	{
		Array<Character> characters;
		for (auto&& [charcode, glyph] : asset->m_Glyphs)
		{
			::Character& character = characters.Emplace();
			character.m_AdvanceX = glyph.m_AdvanceX;
			character.m_AdvanceY = glyph.m_AdvanceY;
			character.m_Character = str::String(1, charcode);
			character.m_SourceFile = glyph.m_SourceFile;
		}
		visitor.Visit(strCharacters, characters, {});
	}

	return true;
}

bool eng::FontAssetLoader::Load(FontAsset* asset, eng::Visitor& visitor) const
{
	// #todo: assert file exists
	// #todo: fallback to default on failure

	visitor.Visit(strWidth, asset->m_Width, 0);
	visitor.Visit(strHeight, asset->m_Height, 0);
	visitor.Visit(strPixelRange, asset->m_PixelRange, 0);

	Array<Character> characters;
	visitor.Visit(strCharacters, characters, {});
	if (!characters.IsEmpty())
	{
		const int32 count = characters.GetCount();
		glGenTextures(1, &asset->m_TextureId);
		glBindTexture(GL_TEXTURE_2D_ARRAY, asset->m_TextureId);
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, asset->m_Width, asset->m_Height, count);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		for (auto&& [depth, character] : enumerate::Forward(characters))
		{
			eng::Charcode charcode = character.m_Character[0];
			eng::Glyph& glyph = asset->m_Glyphs.Emplace(charcode);
			glyph.m_AdvanceX = character.m_AdvanceX;
			glyph.m_AdvanceY = character.m_AdvanceY;
			glyph.m_Depth = static_cast<float>(depth);
			glyph.m_SourceFile = std::move(character.m_SourceFile);

			const str::Path filepath = str::Path(str::EPath::Assets, glyph.m_SourceFile);
			if (!filepath.IsEmpty())
			{
				int32 width, height, channels;
				stbi_set_flip_vertically_on_load(true);
				unsigned char* data = stbi_load(filepath.ToChar(), &width, &height, &channels, STBI_rgb_alpha);
				Z_PANIC(data, "STB Image: Failed to load file [{}]!", filepath.ToChar());

				const int32 mipmap = 0;
				const int32 offsetX = 0, offsetY = 0, offsetZ = static_cast<int32>(glyph.m_Depth);
				glTexSubImage3D(
					GL_TEXTURE_2D_ARRAY,
					mipmap,
					offsetX, offsetY, offsetZ,
					asset->m_Width, asset->m_Height, 1,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					data);

				stbi_image_free(data);
			}
		}
	}

	return true;
}

bool eng::FontAssetLoader::Import(FontAsset* asset, const str::Path& filepath) const
{
	FT_Library library;
	if (const FT_Error error = FT_Init_FreeType(&library))
	{
		Z_LOG(ELog::Debug, "Unable to initialize font library!");
		return false;
	}

	FT_Face face;
	if (const FT_Error error = FT_New_Face(library, filepath.ToChar(), -1, &face))
	{
		switch (error)
		{
		case FT_Err_Unknown_File_Format:
			Z_LOG(ELog::Debug, "Unsupported font format.");
			break;
		}

		return false;
	}

	const int32 fontCount = face->num_faces;
	for (int32 fontIndex = 0; fontIndex < fontCount; ++fontIndex)
	{
		FT_New_Face(library, filepath.ToChar(), fontIndex, &face);
		FT_Select_Charmap(face, FT_ENCODING_UNICODE);

		FT_ULong charcode;
		FT_UInt glyphIndex;
		charcode = FT_Get_First_Char(face, &glyphIndex);
		for (; glyphIndex != 0; charcode = FT_Get_Next_Char(face, charcode, &glyphIndex))
		{
			if (const FT_Error error = FT_Load_Glyph(face, glyphIndex, 0))
				continue;

			eng::Charcode key = static_cast<char>(charcode);
			eng::Glyph& glyph = asset->m_Glyphs.Emplace(key);
			glyph.m_AdvanceX = face->glyph->advance.x;
			glyph.m_AdvanceY = face->glyph->advance.y;
			glyph.m_SourceFile = filepath;
		}
	}

	// #todo: generate charcode textures

	return true;
}
