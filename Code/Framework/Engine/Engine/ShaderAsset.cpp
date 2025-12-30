#include "EnginePCH.h"
#include "Engine/ShaderAsset.h"

#include "Core/Parse.h"
#include "Engine/AssetManager.h"
#include "Engine/TomlHelpers.h"
#include "Engine/Visitor.h"

#include <GLEW/glew.h>

#include <iostream>
#include <regex>

namespace
{
	const str::Name strFragment = NAME("m_Fragment");
	const str::Name strGeometry = NAME("m_Geometry");
	const str::Name strVertex = NAME("m_Vertex");

	GLenum glCheckError_(const char* file, int line)
	{
		GLenum errorCode;
		if ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			}
			std::cout << error << " | " << file << " (" << line << ")" << std::endl;
		}
		return errorCode;
	}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 
}

void eng::ShaderAssetLoader::Bindings(const uint32 programId, ShaderAsset& asset)
{
	PROFILE_FUNCTION();

	auto ATTRIBUTE = [&programId](const char* name, Optional<uint32>& out_Value)
	{
		const int32 location = glGetAttribLocation(programId, name);
		if (location != -1) out_Value = location;
	};

	auto UNIFORM = [&programId](const char* name, Optional<uint32>& out_Value)
	{
		const int32 location = glGetUniformLocation(programId, name);
		if (location != -1) out_Value = location;
	};

	// Per Vertex (Attributes)
	ATTRIBUTE("a_Colour", asset.a_Colour);
	ATTRIBUTE("a_Normal", asset.a_Normal);
	ATTRIBUTE("a_TexCoords", asset.a_TexCoords);
	ATTRIBUTE("a_Vertex", asset.a_Vertex);

	// Per Instance (Attributes)
	ATTRIBUTE("i_Colour", asset.i_Colour);
	ATTRIBUTE("i_Model", asset.i_Model);
	ATTRIBUTE("i_Position", asset.i_Position);
	ATTRIBUTE("i_TexDepth", asset.i_TexDepth);
	ATTRIBUTE("i_TexParams", asset.i_TexParams);

	// Per Shader (Uniforms)
	// - Camera
	UNIFORM("u_CameraProj", asset.u_CameraProj);
	UNIFORM("u_CameraView", asset.u_CameraView);

	// - Light Ambient
	UNIFORM("u_LightAmbient_Colour", asset.u_LightAmbient_Colour);

	// - Light Directional
	UNIFORM("u_LightDirectional_Colour", asset.u_LightDirectional_Colour);
	UNIFORM("u_LightDirectional_Direction", asset.u_LightDirectional_Direction);

	// - Light Point
	UNIFORM("u_LightPoint_Range", asset.u_LightPoint_Range);
	UNIFORM("u_LightPoint_Colour", asset.u_LightPoint_Colour);
	UNIFORM("u_LightPoint_Position", asset.u_LightPoint_Position);

	// - Textures
	UNIFORM("u_Texture", asset.u_Texture);
	UNIFORM("u_Texture_Depth", asset.u_Texture_Depth);
	UNIFORM("u_Texture_ShadowMap", asset.u_Texture_ShadowMap);

	// - Other
	UNIFORM("u_Model", asset.u_Model);
	UNIFORM("u_PixelRange", asset.u_PixelRange);
	UNIFORM("u_Resolution", asset.u_Resolution);
	UNIFORM("u_Time", asset.u_Time);
	UNIFORM("u_Transform", asset.u_Transform);
	UNIFORM("u_Translate", asset.u_Translate);
}

uint32 eng::ShaderAssetLoader::Compile(uint32 shaderType, const str::StringView& data)
{
	PROFILE_FUNCTION();

	uint32 shaderId = glCreateShader(shaderType);

	int32 dataSize = static_cast<int32>(data.size());
	const char* dataChar = data.data();
	glShaderSource(shaderId, 1, &dataChar, &dataSize);
	glCompileShader(shaderId);

	GLint result;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int32 length;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);

		GLchar* strInfoLog = new GLchar[length + 1];
		glGetShaderInfoLog(shaderId, length, &length, strInfoLog);

		Z_LOG(ELog::Debug, "Compilation error in shader: {}", strInfoLog);
		delete[] strInfoLog;

		Z_PANIC(false, "Failed to compile shader!");
	}

	return shaderId;
}

bool eng::ShaderAssetLoader::Bind(eng::ShaderAsset& asset) const
{
	PROFILE_FUNCTION();

	asset.m_ProgramId = glCreateProgram();

	uint32 fragmentShaderId = 0;
	if (!asset.m_Fragment.empty())
	{
		fragmentShaderId = Compile(GL_FRAGMENT_SHADER, asset.m_Fragment);
		glAttachShader(asset.m_ProgramId, fragmentShaderId);
	}

	uint32 geometryShaderId = 0;
	if (!asset.m_Geometry.empty())
	{
		geometryShaderId = Compile(GL_GEOMETRY_SHADER, asset.m_Geometry);
		glAttachShader(asset.m_ProgramId, geometryShaderId);
	}

	uint32 vertexShaderId = 0;
	if (!asset.m_Vertex.empty())
	{
		vertexShaderId = Compile(GL_VERTEX_SHADER, asset.m_Vertex);
		glAttachShader(asset.m_ProgramId, vertexShaderId);
	}

	glLinkProgram(asset.m_ProgramId);
	glValidateProgram(asset.m_ProgramId);

	if (vertexShaderId)
		glDeleteShader(vertexShaderId);
	if (fragmentShaderId)
		glDeleteShader(fragmentShaderId);
	if (geometryShaderId)
		glDeleteShader(geometryShaderId);

	Bindings(asset.m_ProgramId, asset);
	return true;
}

bool eng::ShaderAssetLoader::Unbind(eng::ShaderAsset& asset) const
{
	PROFILE_FUNCTION();

	glDeleteProgram(asset.m_ProgramId);
	return true;
}

bool eng::ShaderAssetLoader::Load(eng::ShaderAsset& asset, eng::Visitor& visitor) const
{
	PROFILE_FUNCTION();
	visitor.Read(strFragment, asset.m_Fragment, {});
	visitor.Read(strGeometry, asset.m_Geometry, {});
	visitor.Read(strVertex, asset.m_Vertex, {});
	return true;
}