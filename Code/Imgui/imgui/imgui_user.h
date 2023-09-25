#pragma once

#include <Core/Types.h>
#include <Core/Vector.h>

struct ImVec2;
struct ImVec4;
typedef int ImDrawFlags;
typedef int ImGuiSliderFlags;
typedef unsigned int ImU32;

namespace imgui
{
	void AddRect(const Vector2f& min, const Vector2f& max, const Vector4f& colour, float rounding = 0.0f, float thickness = 1.0f, ImDrawFlags flags = 0);

	void Bullet();
	bool BulletHeader(const char* label, const bool selected = false);

	bool DragUInt(const char* label, uint32* v, float v_speed = 1.0f, uint32 v_min = 0, uint32 v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragUInt2(const char* label, uint32 v[2], float v_speed = 1.0f, uint32 v_min = 0, uint32 v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragUInt3(const char* label, uint32 v[3], float v_speed = 1.0f, uint32 v_min = 0, uint32 v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragUInt4(const char* label, uint32 v[4], float v_speed = 1.0f, uint32 v_min = 0, uint32 v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);

	void Image(uint32 textureId, const Vector2f& size, const Vector2f& uv0 = Vector2f::Zero, const Vector2f& uv1 = Vector2f::One);
	
	/// \brief Converts a hexadecimal colour to an ImVec4 colour.
	/// Example: 0xFF00FF00 -> { 1.f, 0.f, 1.f, 0.f }
	ImVec4 ToColour(const int32 hexadecimal);

	/// \brief Converts a RGBA hexadecimal colour to a ImU32 colour.
	ImU32 ToColour32(const int32 hexadecimal);
}