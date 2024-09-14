#include "imgui/imgui_user.h"

#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/Path.h"
#include "Core/String.h"
#include "Math/AABB.h"
#include "Math/Quaternion.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"

#include <algorithm>

namespace
{
	float DistanceSqr(const ImVec2& from, const ImVec2& to)
	{
		return (from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y);
	}

	ImVec2 Remap(const ImVec2& value, const ImVec2& fromA, const ImVec2& fromB, const ImVec2& toA, const ImVec2& toB)
	{
		return ImVec2(
			toA.x + (value.x - fromA.x) * (toB.x - toA.x) / (fromB.x - fromA.x),
			toA.y + (value.y - fromA.y) * (toB.y - toA.y) / (fromB.y - fromA.y));
	}

	ImVec2 ToRangeHalf(const ImRect& frame_bb, const ImVec2& spacing)
	{
		constexpr float distance = 100.f;
		ImVec2 count = (frame_bb.Max - frame_bb.Min) / distance;
		ImVec2 range_half;
		range_half.x = spacing.x * count.x * 0.5f;
		range_half.y = spacing.y * count.y * 0.5f;
		return range_half;
	}

	ImVec2 ToLocal(const Vector2f& value, const ImRect& inner_bb, const ImVec2& range_min, const ImVec2& range_max)
	{
		const ImVec2 v = ImVec2(value.x, value.y);
		const ImVec2 t = Remap(v, range_min, range_max, ImVec2(0, 1), ImVec2(1, 0));
		return Remap(t, ImVec2(0, 0), ImVec2(1, 1), inner_bb.Min, inner_bb.Max);
	}
}

imgui::RaiiID::RaiiID(const char* id) 
{ 
	ImGui::PushID(id); 
}

imgui::RaiiID::~RaiiID()
{ 
	ImGui::PopID(); 
}

imgui::RaiiDisable::RaiiDisable() 
{ 
	ImGui::BeginDisabled(); 
}

imgui::RaiiDisable::~RaiiDisable()
{ 
	ImGui::EndDisabled(); 
}

void imgui::AddRect(const AABB2f& value, Vector4f colour, float rounding, float thickness, ImDrawFlags flags)
{
	const Vector2f& min = value.m_Min;
	const Vector2f& max = value.m_Max;

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	const ImU32 colourHex = ImColor(colour.x, colour.y, colour.z, colour.w);
	drawList->AddRect({ min.x, min.y }, { max.x, max.y }, colourHex, rounding, flags, thickness);
}

void imgui::Bullet()
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGui::AlignTextToFramePadding();

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const float line_height = ImMax(ImMin(window->DC.CurrLineSize.y, g.FontSize + g.Style.FramePadding.y * 2), g.FontSize);
	const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(g.FontSize, line_height));
	ImGui::ItemSize(bb);
	if (!ImGui::ItemAdd(bb, 0))
	{
		ImGui::SameLine(0, style.FramePadding.x * 2);
		return;
	}

	// Render and stay on same line
	ImU32 text_col = ImGui::GetColorU32(ImGuiCol_Text);
	ImGui::RenderBullet(window->DrawList, bb.Min + ImVec2(style.FramePadding.x + g.FontSize * 0.5f, line_height * 0.5f), text_col);
	ImGui::SameLine(0, style.FramePadding.x * 3.0f);
}

bool imgui::BulletHeader(const char* label, const bool selected /*= false*/)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	ImGuiID id = window->GetID(label);

	const ImGuiStyle& style = g.Style;
	const ImVec2 padding = style.FramePadding;

	const char* label_end = ImGui::FindRenderedTextEnd(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, label_end, false);

	// We vertically grow up to current line height up the typical widget height.
	const float frame_height = ImMax(ImMin(window->DC.CurrLineSize.y, g.FontSize + style.FramePadding.y * 2), label_size.y + padding.y * 2);
	ImRect frame_bb;
	frame_bb.Min.x = window->WorkRect.Min.x;
	frame_bb.Min.y = window->DC.CursorPos.y;
	frame_bb.Max.x = window->WorkRect.Max.x;
	frame_bb.Max.y = window->DC.CursorPos.y + frame_height;
	frame_bb.Min.x -= IM_FLOOR(window->WindowPadding.x * 0.5f - 1.0f);
	frame_bb.Max.x += IM_FLOOR(window->WindowPadding.x * 0.5f);

	const float text_offset_x = g.FontSize + (padding.x * 3);
	const float text_offset_y = ImMax(padding.y, window->DC.CurrLineTextBaseOffset);
	const float text_width = g.FontSize + (label_size.x > 0.0f ? label_size.x + padding.x * 2 : 0.0f);
	ImVec2 text_pos(window->DC.CursorPos.x + text_offset_x, window->DC.CursorPos.y + text_offset_y);
	ImGui::ItemSize(ImVec2(text_width, frame_height), padding.y);

	ImRect interact_bb = frame_bb;
	bool item_add = ImGui::ItemAdd(interact_bb, id);
	g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasDisplayRect;
	g.LastItemData.DisplayRect = frame_bb;

	ImGuiButtonFlags button_flags = ImGuiButtonFlags_PressedOnClickRelease;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(interact_bb, id, &hovered, &held, button_flags);

	// Render
	const ImU32 text_col = ImGui::GetColorU32(ImGuiCol_Text);
	ImGuiNavHighlightFlags nav_highlight_flags = ImGuiNavHighlightFlags_TypeThin;
	{
		const auto bg_flag = 
			(hovered && held) ? ImGuiCol_HeaderActive :
			(hovered) ? ImGuiCol_HeaderHovered : 
			(selected) ? ImGuiCol_HeaderActive : ImGuiCol_Header;

		const ImU32 bg_col = ImGui::GetColorU32(bg_flag);
		ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, bg_col, true, style.FrameRounding);
		ImGui::RenderNavHighlight(frame_bb, id, nav_highlight_flags);

		if (selected || pressed)
			ImGui::RenderArrow(window->DrawList, ImVec2(text_pos.x - text_offset_x + padding.x, text_pos.y + 1.6f), text_col, ImGuiDir_Right, 0.8f);
		else
			ImGui::RenderBullet(window->DrawList, ImVec2(text_pos.x - text_offset_x * 0.60f, text_pos.y + g.FontSize * 0.5f), text_col);

		ImGui::RenderTextClipped(text_pos, frame_bb.Max, label, label_end, &label_size);
	}

	return pressed;
}

bool imgui::Checkbox(const char* label, bool& value)
{
	return ImGui::Checkbox(label, &value);
}

bool imgui::Checkbox(const char* label, const bool& value)
{
	RaiiDisable disable;
	return ImGui::Checkbox(label, const_cast<bool*>(&value));
}

bool imgui::DragFloat(const char* label, float& value, float speed, float min, float max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragFloat(label, &value, speed, min, max, format, flags);
}

bool imgui::DragFloat(const char* label, const float& value, float speed, float min, float max, const char* format, ImGuiSliderFlags flags)
{
	RaiiDisable disable;
	return ImGui::DragFloat(label, const_cast<float*>(&value), speed, min, max, format, flags);
}

bool imgui::DragDouble(const char* label, double& value, float speed, double min, double max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragScalar(label, ImGuiDataType_Double, &value, speed, &min, &max, format, flags);
}

bool imgui::DragDouble(const char* label, const double& value, float speed, double min, double max, const char* format, ImGuiSliderFlags flags)
{
	RaiiDisable disable;
	return ImGui::DragScalar(label, ImGuiDataType_Double, const_cast<double*>(&value), speed, &min, &max, format, flags);
}

bool imgui::DragInt(const char* label, int8& value, float speed, int8 min, int8 max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragScalar(label, ImGuiDataType_S8, &value, speed, &min, &max, format, flags);
}

bool imgui::DragInt(const char* label, int16& value, float speed, int16 min, int16 max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragScalar(label, ImGuiDataType_S16, &value, speed, &min, &max, format, flags);
}

bool imgui::DragInt(const char* label, int32& value, float speed, int32 min, int32 max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragInt(label, &value, speed, min, max, format, flags);
}

bool imgui::DragInt(const char* label, int64& value, float speed, int64 min, int64 max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragScalar(label, ImGuiDataType_S64, &value, speed, &min, &max, format, flags);
}

bool imgui::DragInt(const char* label, const int8& value, float speed, int8 min, int8 max, const char* format, ImGuiSliderFlags flags)
{
	RaiiDisable disable;
	return ImGui::DragScalar(label, ImGuiDataType_S8, const_cast<int8*>(&value), speed, &min, &max, format, flags);
}

bool imgui::DragInt(const char* label, const int16& value, float speed, int16 min, int16 max, const char* format, ImGuiSliderFlags flags)
{
	RaiiDisable disable;
	return ImGui::DragScalar(label, ImGuiDataType_S16, const_cast<int16*>(&value), speed, &min, &max, format, flags);
}

bool imgui::DragInt(const char* label, const int32& value, float speed, int32 min, int32 max, const char* format, ImGuiSliderFlags flags)
{
	RaiiDisable disable;
	return ImGui::DragInt(label, const_cast<int32*>(&value), speed, min, max, format, flags);
}

bool imgui::DragInt(const char* label, const int64& value, float speed, int64 min, int64 max, const char* format, ImGuiSliderFlags flags)
{
	RaiiDisable disable;
	return ImGui::DragScalar(label, ImGuiDataType_S64, const_cast<int64*>(&value), speed, &min, &max, format, flags);
}

bool imgui::DragUInt(const char* label, uint8& value, float speed, uint8 min, uint8 max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragScalar(label, ImGuiDataType_U8, &value, speed, &min, &max, format, flags);
}

bool imgui::DragUInt(const char* label, uint16& value, float speed, uint16 min, uint16 max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragScalar(label, ImGuiDataType_U16, &value, speed, &min, &max, format, flags);
}

bool imgui::DragUInt(const char* label, uint32& value, float speed, uint32 min, uint32 max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragScalar(label, ImGuiDataType_U32, &value, speed, &min, &max, format, flags);
}

bool imgui::DragUInt(const char* label, uint64& value, float speed, uint64 min, uint64 max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragScalar(label, ImGuiDataType_U64, &value, speed, &min, &max, format, flags);
}

bool imgui::DragUInt(const char* label, const uint8& value, float speed, uint8 min, uint8 max, const char* format, ImGuiSliderFlags flags)
{
	RaiiDisable disable;
	return ImGui::DragScalar(label, ImGuiDataType_U8, const_cast<uint8*>(&value), speed, &min, &max, format, flags);
}

bool imgui::DragUInt(const char* label, const uint16& value, float speed, uint16 min, uint16 max, const char* format, ImGuiSliderFlags flags)
{
	RaiiDisable disable;
	return ImGui::DragScalar(label, ImGuiDataType_U16, const_cast<uint16*>(&value), speed, &min, &max, format, flags);
}

bool imgui::DragUInt(const char* label, const uint32& value, float speed, uint32 min, uint32 max, const char* format, ImGuiSliderFlags flags)
{
	RaiiDisable disable;
	return ImGui::DragScalar(label, ImGuiDataType_U32, const_cast<uint32*>(&value), speed, &min, &max, format, flags);
}

bool imgui::DragUInt(const char* label, const uint64& value, float speed, uint64 min, uint64 max, const char* format, ImGuiSliderFlags flags)
{
	RaiiDisable disable;
	return ImGui::DragScalar(label, ImGuiDataType_U64, const_cast<uint64*>(&value), speed, &min, &max, format, flags);
}

bool imgui::DragQuaternion(const char* label, Quaternion& value, float speed, float min, float max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragFloat4(label, &value.x, speed, min, max, format, flags);
}

bool imgui::DragQuaternion(const char* label, const Quaternion& value, float speed, float min, float max, const char* format, ImGuiSliderFlags flags)
{
	RaiiDisable disable;
	Quaternion& v = const_cast<Quaternion&>(value);
	return ImGui::DragFloat4(label, &v.x, speed, min, max, format, flags);
}

bool imgui::DragRotator(const char* label, Rotator& value, float speed, float min, float max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragFloat3(label, &value.m_Pitch, speed, min, max, format, flags);
}

bool imgui::DragRotator(const char* label, const Rotator& value, float speed, float min, float max, const char* format, ImGuiSliderFlags flags)
{
	RaiiDisable disable;
	Rotator& v = const_cast<Rotator&>(value);
	return ImGui::DragFloat3(label, &v.m_Pitch, speed, min, max, format, flags);
}

bool imgui::DragVector(const char* label, Vector2f& value, float speed, float min, float max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragFloat2(label, &value.x, speed, min, max, format, flags);
}

bool imgui::DragVector(const char* label, Vector2i& value, float speed, int32 min, int32 max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragInt2(label, &value.x, speed, min, max, format, flags);
}

bool imgui::DragVector(const char* label, Vector2u& value, float speed, uint32 min, uint32 max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragScalarN(label, ImGuiDataType_U32, &value.x, 2, speed, &min, &max, format, flags);
}

bool imgui::DragVector(const char* label, Vector3f& value, float speed, float min, float max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragFloat3(label, &value.x, speed, min, max, format, flags);
}

bool imgui::DragVector(const char* label, Vector3i& value, float speed, int32 min, int32 max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragInt3(label, &value.x, speed, min, max, format, flags);
}

bool imgui::DragVector(const char* label, Vector4f& value, float speed, float min, float max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragFloat4(label, &value.x, speed, min, max, format, flags);
}

bool imgui::DragVector(const char* label, const Vector2f& value, float speed, float min, float max, const char* format, ImGuiSliderFlags flags)
{
	RaiiDisable disable;
	Vector2f& v = const_cast<Vector2f&>(value);
	return ImGui::DragFloat2(label, &v.x, speed, min, max, format, flags);
}

bool imgui::DragVector(const char* label, const Vector2i& value, float speed, int32 min, int32 max, const char* format, ImGuiSliderFlags flags)
{
	RaiiDisable disable;
	Vector2i& v = const_cast<Vector2i&>(value);
	return ImGui::DragInt2(label, &v.x, speed, min, max, format, flags);
}

bool imgui::DragVector(const char* label, const Vector2u& value, float speed, uint32 min, uint32 max, const char* format, ImGuiSliderFlags flags)
{
	RaiiDisable disable;
	Vector2u& v = const_cast<Vector2u&>(value);
	return ImGui::DragScalarN(label, ImGuiDataType_U32, &v.x, 2, speed, &min, &max, format, flags);
}

bool imgui::DragVector(const char* label, const Vector3f& value, float speed, float min, float max, const char* format, ImGuiSliderFlags flags)
{
	RaiiDisable disable;
	Vector3f& v = const_cast<Vector3f&>(value);
	return ImGui::DragFloat3(label, &v.x, speed, min, max, format, flags);
}

bool imgui::DragVector(const char* label, const Vector3i& value, float speed, int32 min, int32 max, const char* format, ImGuiSliderFlags flags)
{
	RaiiDisable disable;
	Vector3i& v = const_cast<Vector3i&>(value);
	return ImGui::DragInt3(label, &v.x, speed, min, max, format, flags);
}

bool imgui::DragVector(const char* label, const Vector4f& value, float speed, float min, float max, const char* format, ImGuiSliderFlags flags)
{
	RaiiDisable disable;
	Vector4f& v = const_cast<Vector4f&>(value);
	return ImGui::DragFloat4(label, &v.x, speed, min, max, format, flags);
}

bool imgui::Guid(const char* label, str::Guid& value)
{
	str::String string = value.ToString();
	if (ImGui::InputText(label, &string) && str::Guid::IsValidString(string))
	{
		value = str::Guid::Create(string);
		return true;
	}
	return false;
}

bool imgui::Guid(const char* label, const str::Guid& value)
{
	RaiiDisable disable;
	str::String string = value.ToString();
	return ImGui::InputText(label, &string);
}

bool imgui::Name(const char* label, str::Name& value)
{
	str::String string = str::String(value);
	if (ImGui::InputText(label, &string))
	{
		value = str::Name::Create(string);
		return true;
	}
	return false;
}

bool imgui::Name(const char* label, const str::Name& value)
{
	RaiiDisable disable;
	str::String string = str::String(value);
	return ImGui::InputText(label, &string);
}

bool imgui::Path(const char* label, str::Path& value)
{
	str::String string = str::String(value);
	if (ImGui::InputText(label, &string))
	{
		value = str::Path(string);
		return true;
	}
	return false;
}

bool imgui::Path(const char* label, const str::Path& value)
{
	RaiiDisable disable;
	str::String string = str::String(value);
	return ImGui::InputText(label, &string);
}

bool imgui::String(const char* label, str::String& value)
{
	return ImGui::InputText(label, &value);
}

bool imgui::String(const char* label, const str::String& value)
{
	RaiiDisable disable;
	return ImGui::InputText(label, const_cast<std::string*>(&value));
}

void imgui::Grid(Vector2f graph_size, Vector2f spacing, Vector2f offset)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + graph_size);
	ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
	const ImU32 col_base = ImGui::GetColorU32(ImGuiCol_Border);

	ImVec2 pos0 = frame_bb.Min, pos1 = frame_bb.Min;
	for (float x = 0.f; x <= graph_size.x; x += spacing.x)
	{
		// draw vertical line
		pos0 = { frame_bb.Min.x + offset.x + x, frame_bb.Min.y };
		pos1 = { frame_bb.Min.x + offset.x + x, frame_bb.Max.y };
		window->DrawList->AddLine(pos0, pos1, col_base);

		for (float y = 0.f; y <= graph_size.y; y += spacing.y)
		{
			// draw horizontal line
			pos0 = { frame_bb.Min.x, frame_bb.Min.y + offset.y + y };
			pos1 = { frame_bb.Max.x, frame_bb.Min.y + offset.y + y };
			window->DrawList->AddLine(pos0, pos1, col_base);
		}
	}
}

void imgui::Image(uint32 textureId, Vector2f image_size, Vector2f uv0, Vector2f uv1)
{
	const ImTextureID castedId = (void*)(intptr_t)textureId;
	ImGui::Image(castedId, image_size, { uv0.x, uv1.y }, { uv1.x, uv0.y });
}

void imgui::PlotLines(const char* label, Vector2f* values, int32 values_count, Vector2f graph_size, ImGuiPlotFlags flags)
{
	enum StorageIDs : ImGuiID
	{
		ID_Dragging = 100,
		ID_PositionX,
		ID_PositionY,
		ID_SpacingX,
		ID_SpacingY,
	};

	constexpr ImVec2 padding = ImVec2(0, 0);
	constexpr int32 grid_major = 2;
	constexpr float point_radius = 3.f;
	constexpr float select_radius = 20.f * 20.f;

	ImGuiContext& g = *GImGui;
	ImGuiWindow* parent_window = ImGui::GetCurrentWindow();
	if (parent_window->SkipItems)
		return;

	const ImGuiID id = parent_window->GetID(label);
	const ImGuiStyle& style = g.Style;

	if (graph_size.x == 0.0f)
		graph_size.x = ImGui::GetContentRegionAvail().x;
	if (graph_size.y == 0.0f)
		graph_size.y = (style.FramePadding.y * 2);

	const ImRect frame_bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + graph_size);
	const ImRect inner_bb(frame_bb.Min + padding, frame_bb.Max - padding);
	if (!ImGui::BeginChildFrame(id, graph_size, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		ImGui::EndChild();
		return;
	}

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiStorage* storage = parent_window->DC.StateStorage;
	if (window->SkipItems)
		return;

	ImVec2 position = ImVec2(0.f, 0.f);
	ImVec2 spacing = ImVec2(+1.0f, +1.0f);
	position.x = storage->GetFloat(ID_PositionX, position.x);
	position.y = storage->GetFloat(ID_PositionY, position.y);
	spacing.x = storage->GetFloat(ID_SpacingX, spacing.x);
	spacing.y = storage->GetFloat(ID_SpacingY, spacing.y);

	if (ImGui::IsMouseDragging(ImGuiMouseButton_Right) && ImGui::ItemHoverable(frame_bb, id))
	{
		const ImVec2 delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
		ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);
		position.x -= delta.x * spacing.x * 0.01f;
		position.y += delta.y * spacing.y * 0.01f;
	}

	if (ImGui::GetIO().MouseWheel != 0 && ImGui::ItemHoverable(frame_bb, id))
	{
		spacing *= math::Remap(ImGui::GetIO().MouseWheel, -1.f, 1.f, 2.f, 0.5f);
	}

	storage->SetFloat(ID_PositionX, position.x);
	storage->SetFloat(ID_PositionY, position.y);
	storage->SetFloat(ID_SpacingX, spacing.x);
	storage->SetFloat(ID_SpacingY, spacing.y);

	const ImVec2 range_half = ToRangeHalf(frame_bb, spacing);
	const ImVec2 range_min = position - range_half;
	const ImVec2 range_max = position + range_half;

	if (flags & ImGuiPlotFlags_Grid)
	{
		const ImU32 col_major = ImGui::GetColorU32(ImGuiCol_SeparatorActive);
		const ImU32 col_minor = ImGui::GetColorU32(ImGuiCol_Separator);

		ImVec2 pos0 = frame_bb.Min, pos1 = frame_bb.Min;
		ImVec2 clamp_min = { math::Round(range_min.x, spacing.x), math::Round(range_min.y, spacing.y) };
		ImVec2 clamp_max = { math::Round(range_max.x, spacing.x), math::Round(range_max.y, spacing.y) };

		// draw vertical lines
		pos0.y = frame_bb.Min.y;
		pos1.y = frame_bb.Max.y;
		for (float x = clamp_min.x; x <= clamp_max.x; x += spacing.x)
		{
			const bool isMajor = (math::Round<int32>(x / spacing.x) % grid_major) == 0;
			const ImU32 col = isMajor ? col_major : col_minor;

			pos0.x = math::Remap(x, range_min.x, range_max.x, frame_bb.Min.x, frame_bb.Max.x);
			pos1.x = math::Remap(x, range_min.x, range_max.x, frame_bb.Min.x, frame_bb.Max.x);
			window->DrawList->AddLine(pos0, pos1, col);

			if (isMajor && flags & ImGuiPlotFlags_TextX)
			{
				char buff0[8];
				const char* buff1 = buff0 + ImGui::DataTypeFormatString(buff0, IM_ARRAYSIZE(buff0), ImGuiDataType_Float, &x, " %g");
				ImGui::RenderTextClipped(pos0, frame_bb.Max, buff0, buff1, nullptr);
			}
		}

		// draw horizontal lines
		pos0.x = frame_bb.Min.x;
		pos1.x = frame_bb.Max.x;
		for (float y = clamp_min.y; y <= clamp_max.y; y += spacing.y)
		{
			const bool isMajor = (math::Round<int32>(y / spacing.y) % grid_major) == 0;
			const ImU32 col = isMajor ? col_major : col_minor;

			pos0.y = math::Remap(y, range_min.y, range_max.y, frame_bb.Max.y, frame_bb.Min.y);
			pos1.y = math::Remap(y, range_min.y, range_max.y, frame_bb.Max.y, frame_bb.Min.y);
			window->DrawList->AddLine(pos0, pos1, col);

			if (isMajor && flags & ImGuiPlotFlags_TextX)
			{
				char buff0[8];
				const char* buff1 = buff0 + ImGui::DataTypeFormatString(buff0, IM_ARRAYSIZE(buff0), ImGuiDataType_Float, &y, " %g");
				ImGui::RenderTextClipped(pos0, frame_bb.Max, buff0, buff1, nullptr);
			}
		}
	}

	{
		int idx_hovered = -1;
		float idx_closest = FLT_MAX;

		const bool isActive = ImGui::ItemHoverable(frame_bb, id);
		const ImU32 col_base = ImGui::GetColorU32(ImGuiCol_PlotLines);
		const ImU32 col_hovered = ImGui::GetColorU32(ImGuiCol_PlotLinesHovered);

		ImVec2 p0;
		for (int i = 0; i < values_count; ++i)
		{
			const ImVec2 p1 = ToLocal(values[i], inner_bb, range_min, range_max);

			// closest point
			const float dsqr = DistanceSqr(g.IO.MousePos, p1);
			if (isActive && dsqr < idx_closest && dsqr < select_radius)
			{
				idx_closest = dsqr;
				idx_hovered = i;
			}

			// skip first iteration
			if (i != 0)
				window->DrawList->AddLine(p0, p1, col_base);
			window->DrawList->AddCircleFilled(p1, point_radius, col_base);

			// copy for next point
			p0 = p1;
		}

		// hover highlight
		if (idx_hovered != -1)
		{
			const Vector2f& value = values[idx_hovered];
			const ImVec2 v1 = ImVec2(value.x, value.y);
			const ImVec2 t1 = Remap(v1, range_min, range_max, ImVec2(0, 1), ImVec2(1, 0));
			const ImVec2 p1 = Remap(t1, ImVec2(0, 0), ImVec2(1, 1), inner_bb.Min, inner_bb.Max);

			window->DrawList->AddCircleFilled(p1, point_radius, col_hovered);
		}

		// start dragging
		if (idx_hovered != -1 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			storage->SetInt(ID_Dragging, idx_hovered);

		// stop dragging
		const int idx_dragging = storage->GetInt(ID_Dragging, -1);
		if (idx_dragging != -1 && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			storage->SetInt(ID_Dragging, -1);

		// update dragging
		if (idx_dragging != -1)
		{
			const ImVec2 p1 = g.IO.MousePos;
			const ImVec2 t1 = Remap(p1, inner_bb.Min, inner_bb.Max, ImVec2(0, 1), ImVec2(1, 0));
			const ImVec2 v1 = Remap(t1, ImVec2(0, 0), ImVec2(1, 1), range_min, range_max);

			Vector2f& value = values[idx_dragging];
			value.x = v1.x;
			value.y = v1.y;
		}

		// tooltip
		if (idx_hovered != -1)
		{
			const Vector2f& value = values[idx_hovered];
			ImGui::SetTooltip("[%d] %1.3f, %1.3f", idx_hovered, value.x, value.y);
		}
	}

	ImGui::EndChildFrame();
}

Vector4f imgui::ToColour(const int32 hexadecimal)
{
	const int32 r = (hexadecimal & 0xFF000000) >> 24;
	const int32 g = (hexadecimal & 0x00FF0000) >> 16;
	const int32 b = (hexadecimal & 0x0000FF00) >> 8;
	const int32 a = (hexadecimal & 0x000000FF);
	return Vector4f(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

uint32 imgui::ToColour32(const int32 hexadecimal)
{
	const int32 r = (hexadecimal & 0xFF000000) >> 24;
	const int32 g = (hexadecimal & 0x00FF0000) >> 16;
	const int32 b = (hexadecimal & 0x0000FF00) >> 8;
	const int32 a = (hexadecimal & 0x000000FF);
	return IM_COL32(r, g, b, a);
}