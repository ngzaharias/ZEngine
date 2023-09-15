#include "imgui/imgui_user.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include <algorithm>

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

void imgui::PlotLines(const char* label, float* values, int values_count, ImVec2 graph_size, ImGuiGraphFlags flags)
{
	enum StorageIDs : ImGuiID
	{
		ID_Dragging = 100,
		ID_PointsMinX,
		ID_PointsMinY,
		ID_PointsMaxX,
		ID_PointsMaxY,
	};

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
	const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
	if (!ImGui::BeginChildFrame(id, graph_size, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		ImGui::EndChild();
		return;
	}

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiStorage* storage = parent_window->DC.StateStorage;
	if (window->SkipItems)
		return;

	//ImGui::ItemSize(frame_bb, style.FramePadding.y);
	//if (!ImGui::ItemAdd(frame_bb, 0, &frame_bb))
	//	return;
	//ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

	// adjust range
	ImVec2 points_min = ImVec2(-1.0f, -0.4f);
	ImVec2 points_max = ImVec2(+1.0f, +0.4f);
	if (values_count >= 1)
	{
		for (int i = 0; i < values_count; i += 2)
		{
			const ImVec2 value = ImVec2(values[i + 0], values[i + 1]);
			const bool isNaNX = value.x != value.x;
			if (!isNaNX)
			{
				points_min.x = ImMin(points_min.x, value.x);
				points_max.x = ImMax(points_max.x, value.x);
			}
			const bool isNaNY = value.y != value.y;
			if (!isNaNY)
			{
				points_min.y = ImMin(points_min.y, value.y);
				points_max.y = ImMax(points_max.y, value.y);
			}
		}

		const float width = points_max.x - points_min.x;
		const float height = points_max.y - points_min.y;
	}

	points_min.x = storage->GetFloat(ID_PointsMinX, points_min.x);
	points_min.y = storage->GetFloat(ID_PointsMinY, points_min.y);
	points_max.x = storage->GetFloat(ID_PointsMaxX, points_max.x);
	points_max.y = storage->GetFloat(ID_PointsMaxY, points_max.y);

	if (ImGui::GetIO().MouseWheel != 0 && ImGui::ItemHoverable(frame_bb, id))
	{
		const float scale = powf(2, ImGui::GetIO().MouseWheel);
		points_max.x /= scale;
		points_max.y /= scale;
	}

	storage->SetFloat(ID_PointsMinX, points_min.x);
	storage->SetFloat(ID_PointsMinY, points_min.y);
	storage->SetFloat(ID_PointsMaxX, points_max.x);
	storage->SetFloat(ID_PointsMaxY, points_max.y);

	int idx_hovered = -1;
	float idx_closest = FLT_MAX;
	{
		const bool isActive = ImGui::ItemHoverable(frame_bb, id);
		const ImU32 col_base = ImGui::GetColorU32(ImGuiCol_PlotLines);
		const ImU32 col_hovered = ImGui::GetColorU32(ImGuiCol_PlotLinesHovered);

		ImVec2 v0, t0, p0;
		//ImGui::PushClipRect(frame_bb.Min, frame_bb.Max, true);

		for (int idx1 = 0; idx1 < values_count; idx1 += 2)
		{
			const ImVec2 v1 = ImVec2(values[idx1 + 0], values[idx1 + 1]);
			const ImVec2 t1 = ImVec2(Remap(v1.x, points_min.x, points_max.x, 0.f, 1.f), 1.0f - Remap(v1.y, points_min.y, points_max.y, 0.f, 1.f));
			const ImVec2 p1 = ImLerp(inner_bb.Min, inner_bb.Max, t1);

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
		//ImGui::PopClipRect();

		// hover highlight
		if (idx_hovered != -1)
		{
			const ImVec2 v1 = ImVec2(values[idx_hovered + 0], values[idx_hovered + 1]);
			const ImVec2 t1 = ImVec2(Remap(v1.x, points_min.x, points_max.x, 0.f, 1.f), 1.0f - Remap(v1.y, points_min.y, points_max.y, 0.f, 1.f));
			const ImVec2 p1 = ImLerp(inner_bb.Min, inner_bb.Max, t1);
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
			ImVec2 pos = g.IO.MousePos;
			pos.x = Remap(pos.x, inner_bb.Min.x, inner_bb.Max.x, points_min.x, points_max.x);
			pos.y = Remap(pos.y, inner_bb.Min.y, inner_bb.Max.y, points_min.y, points_max.y);

			values[idx_dragging + 0] = pos.x;
			values[idx_dragging + 1] = points_max.y - pos.y;
		}

		// tooltip
		if (idx_hovered != -1/* && !ImGui::IsMouseDragging(ImGuiMouseButton_Left)*/)
			ImGui::SetTooltip("[%d] %1.3f, %1.3f", idx_hovered / 2, values[idx_hovered + 0], values[idx_hovered + 1]);
	}

	ImGui::EndChildFrame();
}

void imgui::PlotLines(const char* label, ImVec2* values, int values_count, ImVec2 graph_size, ImGuiGraphFlags flags)
{
	ImVec2 dummy;
	PlotLines(label, values_count > 0 ? &values->x : &dummy.x, values_count * 2, graph_size, flags);
}

ImVec4 imgui::ToColour(const int hexadecimal)
{
	const int r = (hexadecimal & 0xFF000000) >> 24;
	const int g = (hexadecimal & 0x00FF0000) >> 16;
	const int b = (hexadecimal & 0x0000FF00) >> 8;
	const int a = (hexadecimal & 0x000000FF);
	return ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

ImU32 imgui::ToColour32(const int hexadecimal)
{
	const int r = (hexadecimal & 0xFF000000) >> 24;
	const int g = (hexadecimal & 0x00FF0000) >> 16;
	const int b = (hexadecimal & 0x0000FF00) >> 8;
	const int a = (hexadecimal & 0x000000FF);
	return IM_COL32(r, g, b, a);
}