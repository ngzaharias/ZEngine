#include "imgui/imgui_user.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include <algorithm>

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