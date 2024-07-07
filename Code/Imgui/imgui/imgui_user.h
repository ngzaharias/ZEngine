#pragma once

#include <Core/String.h>
#include <Core/Types.h>
#include <Core/Vector.h>

class Rect2f;
class Rotator;
typedef int ImDrawFlags;
typedef int ImGuiSliderFlags;

enum ImGuiPlotFlags_
{
	ImGuiPlotFlags_Grid = 1 << 0,
	ImGuiPlotFlags_TextX = 1 << 1,
	ImGuiPlotFlags_TextY = 1 << 2,
};
using ImGuiPlotFlags = int; // -> enum ImGuiPlotFlags_

namespace str
{
	class Guid;
	class Name;
	class Path;
	using String = std::string;
}

namespace imgui
{
	void AddRect(const Rect2f& value, Vector4f colour, float rounding = 0.f, float thickness = 1.f, ImDrawFlags flags = 0);

	void Bullet();
	bool BulletHeader(const char* label, const bool selected = false);
	bool DragUInt(const char* label, uint32* v, float v_speed = 1.0f, uint32 v_min = 0, uint32 v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragUInt2(const char* label, uint32 v[2], float v_speed = 1.0f, uint32 v_min = 0, uint32 v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragUInt3(const char* label, uint32 v[3], float v_speed = 1.0f, uint32 v_min = 0, uint32 v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragUInt4(const char* label, uint32 v[4], float v_speed = 1.0f, uint32 v_min = 0, uint32 v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragRotator(const char* label, Rotator& value, float v_speed = 1.0f, float v_min = 0.f, float v_max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool DragVector(const char* label, Vector2f& value, float v_speed = 1.0f, float v_min = 0.f, float v_max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool DragVector(const char* label, Vector3f& value, float v_speed = 1.0f, float v_min = 0.f, float v_max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool DragVector(const char* label, Vector4f& value, float v_speed = 1.0f, float v_min = 0.f, float v_max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	bool Guid(const char* label, str::Guid& value);
	bool Name(const char* label, str::Name& value);
	bool Path(const char* label, str::Path& value);
	bool String(const char* label, str::String& value);

	void Grid(Vector2f graph_size, Vector2f spacing, Vector2f offset = Vector2f::Zero);

	void Image(uint32 textureId, Vector2f image_size, Vector2f uv0 = Vector2f::Zero, Vector2f uv1 = Vector2f::One);

	void PlotLines(const char* label, Vector2f* values, int32 values_count, Vector2f graph_size = Vector2f::Zero, ImGuiPlotFlags flags = 0);

	/// \brief Converts a hexadecimal colour to a vector.
	/// Example: 0xFF00FF00 -> { 1.f, 0.f, 1.f, 0.f }
	Vector4f ToColour(const int32 hexadecimal);

	/// \brief Converts a RGBA hexadecimal colour to a imgui compatible colour.
	uint32 ToColour32(const int32 hexadecimal);
}