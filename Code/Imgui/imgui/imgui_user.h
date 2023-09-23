#pragma once

#include <Core/Types.h>
#include <Core/Vector.h>

struct ImVec2;
struct ImVec4;
typedef int ImDrawFlags;
typedef unsigned int ImU32;

namespace imgui
{
	void AddRect(const Vector2f& min, const Vector2f& max, const Vector4f& colour, float rounding = 0.0f, float thickness = 1.0f, ImDrawFlags flags = 0);

	void Bullet();
	bool BulletHeader(const char* label, const bool selected = false);

	void Image(uint32 textureId, const Vector2f& size, const Vector2f& uv0 = Vector2f::Zero, const Vector2f& uv1 = Vector2f::One);

	/// \brief Converts a hexadecimal colour to an ImVec4 colour.
	/// Example: 0xFF00FF00 -> { 1.f, 0.f, 1.f, 0.f }
	ImVec4 ToColour(const int32 hexadecimal);

	/// \brief Converts a RGBA hexadecimal colour to a ImU32 colour.
	ImU32 ToColour32(const int32 hexadecimal);
}