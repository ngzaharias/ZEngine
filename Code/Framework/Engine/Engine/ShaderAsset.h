#pragma once

#include "Core/Map.h"
#include "Core/Name.h"
#include "Core/Optional.h"
#include "Core/Types.h"
#include "Engine/Asset.h"
#include "Engine/AssetLoader.h"

namespace eng
{
	class Visitor;
}

namespace eng
{
	struct ShaderAsset : public eng::Asset, eng::DeferredLoad
	{
		str::String m_Fragment = {};
		str::String m_Geometry = {};
		str::String m_Vertex = {};

		uint32 m_ProgramId = 0;

		// Per Vertex (Attributes)
		Optional<uint32> a_Colour = {};
		Optional<uint32> a_Normal = {};
		Optional<uint32> a_TexCoords = {};
		Optional<uint32> a_Vertex = {};

		// Per Instance (Attributes)
		Optional<uint32> i_Colour = {};
		Optional<uint32> i_Model = {};
		Optional<uint32> i_Position = {};
		Optional<uint32> i_TexDepth = {};
		Optional<uint32> i_TexParams = {};

		// Per Shader (Uniforms)
		// - Camera
		Optional<uint32> u_CameraProj = {};
		Optional<uint32> u_CameraView = {};

		// - Light Ambient
		Optional<uint32> u_LightAmbient_Colour = {};
		
		// - Light Directional
		Optional<uint32> u_LightDirectional_Colour = {};
		Optional<uint32> u_LightDirectional_Direction = {};
		
		// - Light Point
		Optional<uint32> u_LightPoint_Range = {};
		Optional<uint32> u_LightPoint_Colour = {};
		Optional<uint32> u_LightPoint_Position = {};

		// - Textures
		Optional<uint32> u_Texture = {};
		Optional<uint32> u_Texture_Depth = {};
		Optional<uint32> u_Texture_ShadowMap = {};

		// - Other
		Optional<uint32> u_Model = {};
		Optional<uint32> u_PixelRange = {};
		Optional<uint32> u_Resolution = {};
		Optional<uint32> u_Time = {};
		Optional<uint32> u_Transform = {};
		Optional<uint32> u_Translate = {};
	};

	class ShaderAssetLoader final : public eng::AssetLoader
	{
	public:
		static void Bindings(const uint32 programId, ShaderAsset& asset);
		static uint32 Compile(uint32 shaderType, const str::StringView& data);

		bool Bind(eng::ShaderAsset& asset) const;
		bool Unbind(eng::ShaderAsset& asset) const;

		bool Load(eng::ShaderAsset& asset, eng::Visitor& visitor) const;
	};
}