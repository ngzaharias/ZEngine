#pragma once

#include <Core/Map.h>
#include <Core/Name.h>
#include <Core/Nullable.h>
#include <Core/Types.h>

#include <Engine/Asset.h>
#include <Engine/AssetLoader.h>

namespace eng
{
	class Visitor;
}

namespace eng
{
	struct ShaderAsset : public eng::Asset
	{
		uint32 m_ProgramId = 0;

		// Per Vertex (Attributes)
		Nullable<uint32> a_Colour = { };
		Nullable<uint32> a_Normal = { };
		Nullable<uint32> a_TexCoords = { };
		Nullable<uint32> a_Vertex = { };

		// Per Instance (Attributes)
		Nullable<uint32> i_Colour = { };
		Nullable<uint32> i_Model = { };
		Nullable<uint32> i_Position = { };
		Nullable<uint32> i_TexDepth = { };
		Nullable<uint32> i_TexParams = { };

		// Per Shader (Uniforms)
		// - Camera
		Nullable<uint32> u_CameraProj = { };
		Nullable<uint32> u_CameraView = { };

		// - Light Ambient
		Nullable<uint32> u_LightAmbient_Colour = { };
		
		// - Light Directional
		Nullable<uint32> u_LightDirectional_Colour = { };
		Nullable<uint32> u_LightDirectional_Direction = { };
		
		// - Light Point
		Nullable<uint32> u_LightPoint_Range = { };
		Nullable<uint32> u_LightPoint_Colour = { };
		Nullable<uint32> u_LightPoint_Position = { };

		// - Textures
		Nullable<uint32> u_Texture = { };
		Nullable<uint32> u_Texture_Depth = { };
		Nullable<uint32> u_Texture_ShadowMap = { };

		// - Other
		Nullable<uint32> u_Model = { };
		Nullable<uint32> u_PixelRange = { };
		Nullable<uint32> u_Resolution = { };
		Nullable<uint32> u_Time = { };
		Nullable<uint32> u_Transform = { };
		Nullable<uint32> u_Translate = { };
	};

	class ShaderAssetLoader final : public eng::AssetLoader
	{
	public:
		static void Bindings(const uint32 programId, ShaderAsset& asset);
		static uint32 Compile(uint32 shaderType, const str::StringView& data);

		bool Load(ShaderAsset* asset, eng::Visitor& visitor) const;
	};
}