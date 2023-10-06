#include "imgui/imgui_user.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"

#include <algorithm>

#include <Core/Guid.h>
#include <Core/Name.h>
#include <Core/Path.h>
#include <Core/Vector.h>

namespace
{
	float DistanceSqr(const ImVec2& from, const ImVec2& to)
	{
		return (from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y);
	}

	float Remap(float value, const float fromA, const float fromB, const float toA, const float toB)
	{
		return toA + (value - fromA) * (toB - toA) / (fromB - fromA);
	}

	ImVec2 Remap(const ImVec2& value, const ImVec2& fromA, const ImVec2& fromB, const ImVec2& toA, const ImVec2& toB)
	{
		return ImVec2(
			toA.x + (value.x - fromA.x) * (toB.x - toA.x) / (fromB.x - fromA.x),
			toA.y + (value.y - fromA.y) * (toB.y - toA.y) / (fromB.y - fromA.y));
	}
}

void imgui::AddRect(const Vector2f& min, const Vector2f& max, const Vector4f& colour, float rounding, float thickness, ImDrawFlags flags)
{
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

bool imgui::DragUInt(const char* label, uint32* v, float v_speed, uint32 v_min, uint32 v_max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragScalar(label, ImGuiDataType_U32, v, v_speed, &v_min, &v_max, format, flags);
}

bool imgui::DragUInt2(const char* label, uint32 v[2], float v_speed, uint32 v_min, uint32 v_max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragScalarN(label, ImGuiDataType_U32, v, 2, v_speed, &v_min, &v_max, format, flags);
}

bool imgui::DragUInt3(const char* label, uint32 v[3], float v_speed, uint32 v_min, uint32 v_max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragScalarN(label, ImGuiDataType_U32, v, 3, v_speed, &v_min, &v_max, format, flags);
}

bool imgui::DragUInt4(const char* label, uint32 v[4], float v_speed, uint32 v_min, uint32 v_max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragScalarN(label, ImGuiDataType_U32, v, 4, v_speed, &v_min, &v_max, format, flags);
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

void imgui::Image(uint32 textureId, const Vector2f& size, const Vector2f& uv0, const Vector2f& uv1)
{
	const ImTextureID castedId = (void*)(intptr_t)textureId;
	ImGui::Image(castedId, { size.x, size.y }, { uv0.x, uv1.y }, { uv1.x, uv0.y });
}

void imgui::PlotLines(const char* label, float* values, int values_count, ImVec2 graph_size, ImGuiGraphFlags flags)
{
	enum StorageIDs : ImGuiID
	{
		ID_Dragging = 100,
		ID_RangeMinX,
		ID_RangeMinY,
		ID_RangeMaxX,
		ID_RangeMaxY,
	};

	constexpr ImVec2 padding = ImVec2(0, 0);
	constexpr float point_radius = 3.f;
	constexpr float select_radius = 20.f * 20.f;

	ImGuiContext& g = *GImGui;
	ImGuiWindow* parent_window = ImGui::GetCurrentWindow();
	if (parent_window->SkipItems)
		return;

	const ImGuiID id = parent_window->GetID(label);
	const ImGuiStyle& style = g.Style;
	if (graph_size.x == 0.0f)
		graph_size.x = ImGui::CalcItemWidth();
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

	// adjust range
	ImVec2 range_min = ImVec2(-1.0f, -0.4f);
	ImVec2 range_max = ImVec2(+1.0f, +0.4f);
	if (values_count >= 1)
	{
		for (int i = 0; i < values_count; i += 2)
		{
			const ImVec2 value = ImVec2(values[i + 0], values[i + 1]);
			const bool isNaNX = value.x != value.x;
			if (!isNaNX)
			{
				range_min.x = ImMin(range_min.x, value.x);
				range_max.x = ImMax(range_max.x, value.x);
			}
			const bool isNaNY = value.y != value.y;
			if (!isNaNY)
			{
				range_min.y = ImMin(range_min.y, value.y);
				range_max.y = ImMax(range_max.y, value.y);
			}
		}

		const float width = range_max.x - range_min.x;
		const float height = range_max.y - range_min.y;
	}

	range_min.x = storage->GetFloat(ID_RangeMinX, range_min.x);
	range_min.y = storage->GetFloat(ID_RangeMinY, range_min.y);
	range_max.x = storage->GetFloat(ID_RangeMaxX, range_max.x);
	range_max.y = storage->GetFloat(ID_RangeMaxY, range_max.y);

	if (ImGui::GetIO().MouseWheel != 0 && ImGui::ItemHoverable(frame_bb, id))
	{
		const float scale = 2.f * ImGui::GetIO().MouseWheel;
		float width = (range_max.x - range_min.x) / scale / 2.f;
		float height = (range_max.y - range_min.y) / scale / 2.f;
		range_min += ImVec2(width, height);
		range_max -= ImVec2(width, height);
	}

	storage->SetFloat(ID_RangeMinX, range_min.x);
	storage->SetFloat(ID_RangeMinY, range_min.y);
	storage->SetFloat(ID_RangeMaxX, range_max.x);
	storage->SetFloat(ID_RangeMaxY, range_max.y);

	int idx_hovered = -1;
	float idx_closest = FLT_MAX;
	{
		const bool isActive = ImGui::ItemHoverable(frame_bb, id);
		const ImU32 col_base = ImGui::GetColorU32(ImGuiCol_PlotLines);
		const ImU32 col_hovered = ImGui::GetColorU32(ImGuiCol_PlotLinesHovered);

		ImVec2 v0, t0, p0;
		for (int idx1 = 0; idx1 < values_count; idx1 += 2)
		{
			const ImVec2 v1 = ImVec2(values[idx1 + 0], values[idx1 + 1]);
			const ImVec2 t1 = Remap(v1, range_min, range_max, ImVec2(0, 1), ImVec2(1, 0));
			const ImVec2 p1 = Remap(t1, ImVec2(0, 0), ImVec2(1, 1), inner_bb.Min, inner_bb.Max);

			// closest point
			const float dsqr = DistanceSqr(g.IO.MousePos, p1);
			if (isActive && dsqr < idx_closest && dsqr < select_radius)
			{
				idx_closest = dsqr;
				idx_hovered = idx1;
			}

			// skip first iteration
			if (idx1 != 0)
				window->DrawList->AddLine(p0, p1, col_base);
			window->DrawList->AddCircleFilled(p1, point_radius, col_base);

			// copy for next point
			p0 = p1;
		}

		// hover highlight
		if (idx_hovered != -1)
		{
			const ImVec2 v1 = ImVec2(values[idx_hovered + 0], values[idx_hovered + 1]);
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

			values[idx_dragging + 0] = v1.x;
			values[idx_dragging + 1] = v1.y;
		}

		// tooltip
		if (idx_hovered != -1)
			ImGui::SetTooltip("[%d] %1.3f, %1.3f", idx_hovered / 2, values[idx_hovered + 0], values[idx_hovered + 1]);
	}

	ImGui::EndChildFrame();
}

void imgui::PlotLines(const char* label, ImVec2* values, int values_count, ImVec2 graph_size, ImGuiGraphFlags flags)
{
	ImVec2 dummy;
	PlotLines(label, values_count > 0 ? &values->x : &dummy.x, values_count * 2, graph_size, flags);
}

ImVec4 imgui::ToColour(const int32 hexadecimal)
{
	const int32 r = (hexadecimal & 0xFF000000) >> 24;
	const int32 g = (hexadecimal & 0x00FF0000) >> 16;
	const int32 b = (hexadecimal & 0x0000FF00) >> 8;
	const int32 a = (hexadecimal & 0x000000FF);
	return ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

ImU32 imgui::ToColour32(const int32 hexadecimal)
{
	const int32 r = (hexadecimal & 0xFF000000) >> 24;
	const int32 g = (hexadecimal & 0x00FF0000) >> 16;
	const int32 b = (hexadecimal & 0x0000FF00) >> 8;
	const int32 a = (hexadecimal & 0x000000FF);
	return IM_COL32(r, g, b, a);
}