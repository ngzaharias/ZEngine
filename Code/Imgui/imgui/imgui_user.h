#pragma once

#include "Core/String.h"
#include "Core/Types.h"
#include "Math/Vector.h"

class AABB2f;
class Colour;
class Quaternion;
class Rotator;
typedef int ImDrawFlags;
typedef int ImGuiCond;
typedef int ImGuiSliderFlags;
typedef int ImGuiSelectableFlags;

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
}

namespace imgui
{
	struct RaiiID
	{
		RaiiID(const void* id);
		RaiiID(const char* id);
		RaiiID(const int32 id);
		RaiiID(const str::String& id);
		~RaiiID();
	};

	struct RaiiDisable
	{
		RaiiDisable();
		~RaiiDisable();
	};

	struct RaiiIndent
	{
		RaiiIndent(int32 column = -1);
		~RaiiIndent();
	private:
		int32 m_Column;
	};

	void AddRect(const AABB2f& value, Vector4f colour, float rounding = 0.f, float thickness = 1.f, ImDrawFlags flags = 0);

	void Bullet();
	bool BulletHeader(const char* label, const bool selected = false);
	
	bool Checkbox(const char* label, bool& value);
	bool Checkbox(const char* label, const bool& value);

	bool DragColour(const char* label, Colour& value, float speed = 1.f, float min = 0.f, float max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool DragColour(const char* label, const Colour& value, float speed = 1.f, float min = 0.f, float max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	bool DragFloat(const char* label, float& value, float speed = 1.f, float min = 0, float max = 0, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool DragFloat(const char* label, const float& value, float speed = 1.f, float min = 0, float max = 0, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	bool DragDouble(const char* label, double& value, float speed = 1.f, double min = 0, double max = 0, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool DragDouble(const char* label, const double& value, float speed = 1.f, double min = 0, double max = 0, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	bool DragInt(const char* label, int8& value, float speed = 1.f, int8 min = 0, int8 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragInt(const char* label, int16& value, float speed = 1.f, int16 min = 0, int16 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragInt(const char* label, int32& value, float speed = 1.f, int32 min = 0, int32 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragInt(const char* label, int64& value, float speed = 1.f, int64 min = 0, int64 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragInt(const char* label, const int8& value, float speed = 1.f, int8 min = 0, int8 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragInt(const char* label, const int16& value, float speed = 1.f, int16 min = 0, int16 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragInt(const char* label, const int32& value, float speed = 1.f, int32 min = 0, int32 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragInt(const char* label, const int64& value, float speed = 1.f, int64 min = 0, int64 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);

	bool DragUInt(const char* label, uint8& value, float speed = 1.f, uint8 min = 0, uint8 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragUInt(const char* label, uint16& value, float speed = 1.f, uint16 min = 0, uint16 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragUInt(const char* label, uint32& value, float speed = 1.f, uint32 min = 0, uint32 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragUInt(const char* label, uint64& value, float speed = 1.f, uint64 min = 0, uint64 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragUInt(const char* label, const uint8& value, float speed = 1.f, uint8 min = 0, uint8 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragUInt(const char* label, const uint16& value, float speed = 1.f, uint16 min = 0, uint16 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragUInt(const char* label, const uint32& value, float speed = 1.f, uint32 min = 0, uint32 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragUInt(const char* label, const uint64& value, float speed = 1.f, uint64 min = 0, uint64 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);

	bool DragQuaternion(const char* label, Quaternion& value, float speed = 1.f, float min = 0.f, float max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool DragQuaternion(const char* label, const Quaternion& value, float speed = 1.f, float min = 0.f, float max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	bool DragRotator(const char* label, Rotator& value, float speed = 1.f, float min = 0.f, float max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool DragRotator(const char* label, const Rotator& value, float speed = 1.f, float min = 0.f, float max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	bool DragVector(const char* label, Vector2f& value, float speed = 1.f, float min = 0.f, float max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool DragVector(const char* label, Vector2i& value, float speed = 1.f, int32 min = 0, int32 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragVector(const char* label, Vector2u& value, float speed = 1.f, uint32 min = 0, uint32 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragVector(const char* label, Vector3f& value, float speed = 1.f, float min = 0.f, float max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool DragVector(const char* label, Vector3i& value, float speed = 1.f, int32 min = 0, int32 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragVector(const char* label, Vector4f& value, float speed = 1.f, float min = 0.f, float max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool DragVector(const char* label, const Vector2f& value, float speed = 1.f, float min = 0.f, float max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool DragVector(const char* label, const Vector2i& value, float speed = 1.f, int32 min = 0, int32 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragVector(const char* label, const Vector2u& value, float speed = 1.f, uint32 min = 0, uint32 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragVector(const char* label, const Vector3f& value, float speed = 1.f, float min = 0.f, float max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool DragVector(const char* label, const Vector3i& value, float speed = 1.f, int32 min = 0, int32 max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool DragVector(const char* label, const Vector4f& value, float speed = 1.f, float min = 0.f, float max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	bool InputText(const char* label, str::Guid& value);
	bool InputText(const char* label, const str::Guid& value);
	bool InputText(const char* label, str::Name& value);
	bool InputText(const char* label, const str::Name& value);
	bool InputText(const char* label, str::Path& value);
	bool InputText(const char* label, const str::Path& value);
	bool InputText(const char* label, str::String& value);
	bool InputText(const char* label, const str::String& value);

	void Grid(Vector2f graph_size, Vector2f spacing, Vector2f offset = Vector2f::Zero);

	void Image(uint32 textureId, Vector2f image_size, Vector2f uv0 = Vector2f::Zero, Vector2f uv1 = Vector2f::One);

	void PlotLines(const char* label, Vector2f* values, int32 values_count, Vector2f graph_size = Vector2f::Zero, ImGuiPlotFlags flags = 0);

	bool Selectable(const str::String& label, bool selected = false, ImGuiSelectableFlags flags = 0, const Vector2f size = Vector2f::Zero);

	void Text(const str::Guid& value);
	void Text(const str::Name& value);
	void Text(const str::Path& value);
	void Text(const str::String& value);

	void SetNextWindowPos(const Vector2f& pos);
	void SetNextWindowPos(const Vector2f& pos, const ImGuiCond cond);
	void SetNextWindowPos(const Vector2f& pos, const Vector2f& pivot);
	void SetNextWindowPos(const Vector2f& pos, const Vector2f& pivot, const ImGuiCond cond);
	void SetNextWindowSize(const Vector2f& size, ImGuiCond cond = 0);

	/// \brief Converts a hexadecimal colour to a vector.
	/// Example: 0xFF00FF00 -> { 1.f, 0.f, 1.f, 0.f }
	Vector4f ToColour(const int32 hexadecimal);

	/// \brief Converts a RGBA hexadecimal colour to a imgui compatible colour.
	uint32 ToColour32(const int32 hexadecimal);
}