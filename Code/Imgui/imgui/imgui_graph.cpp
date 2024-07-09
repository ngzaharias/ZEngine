#include "imgui/imgui_graph.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_user.h"
#include "imgui/imgui_stdlib.h"

#include "Core/AABB.h"
#include "Core/Circle.h"
#include "Core/CollisionMath.h"
#include "Core/Line.h"
#include "Core/Ray.h"
#include "Core/Segment.h"
#include "Core/Triangle.h"
#include "Core/VectorMath.h"

namespace
{
	constexpr ImVec2 padding = ImVec2(0, 0);
	constexpr int32 grid_major = 2;
	constexpr float controlRadius = 3.f;
	constexpr float selectRadius = 20.f;
	constexpr float selectRadiusSqr = selectRadius * selectRadius;
	constexpr float scaleMultiplier = 100.f;

	enum StorageIDs : ImGuiID
	{
		ID_Zero = 100,
		ID_Scale,
		ID_Spacing,
		ID_DragIdx,
		ID_ShapeIdx,
		ID_PositionX,
		ID_PositionY,
		ID_FrameMinX,
		ID_FrameMinY,
		ID_FrameMaxX,
		ID_FrameMaxY,
		ID_InnerMinX,
		ID_InnerMinY,
		ID_InnerMaxX,
		ID_InnerMaxY,
		ID_RangeMinX,
		ID_RangeMinY,
		ID_RangeMaxX,
		ID_RangeMaxY,
	};

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

	bool IsDragging(ImGuiMouseButton button, int dragIdx)
	{
		if (!ImGui::IsMouseDragging(button))
			return false;

		ImGuiStorage* storage = ImGui::GetStateStorage();
		return dragIdx == storage->GetInt(ID_DragIdx);
	}

	bool IsOverlappingControl(const ImVec2& mousePos, const ImVec2& controlPos)
	{
		const ImVec2 vector = controlPos - mousePos;
		const float distanceSqr = vector.x * vector.x + vector.y * vector.y;
		return distanceSqr < selectRadiusSqr;
	}

	void AssignDrag(int shapeIdx)
	{
		ImGuiStorage* storage = ImGui::GetStateStorage();
		storage->SetInt(ID_DragIdx, shapeIdx);
	}

	ImVec2 ConsumeDrag(ImGuiMouseButton button)
	{
		const ImVec2 delta = ImGui::GetMouseDragDelta(button);
		ImGui::ResetMouseDragDelta(button);
		return delta;
	}

	int GenerateDragIndex()
	{
		ImGuiStorage* storage = ImGui::GetStateStorage();
		const int shapeIdx = storage->GetInt(ID_ShapeIdx) + 1;
		storage->SetInt(ID_ShapeIdx, shapeIdx);
		return shapeIdx;
	}

	ImRect GetInner()
	{
		ImGuiStorage* storage = ImGui::GetStateStorage();

		ImRect value;
		value.Min.x = storage->GetFloat(ID_InnerMinX);
		value.Min.y = storage->GetFloat(ID_InnerMinY);
		value.Max.x = storage->GetFloat(ID_InnerMaxX);
		value.Max.y = storage->GetFloat(ID_InnerMaxY);
		return value;
	}

	ImRect GetRange()
	{
		ImGuiStorage* storage = ImGui::GetStateStorage();

		ImRect value;
		value.Min.x = storage->GetFloat(ID_RangeMinX);
		value.Min.y = storage->GetFloat(ID_RangeMinY);
		value.Max.x = storage->GetFloat(ID_RangeMaxX);
		value.Max.y = storage->GetFloat(ID_RangeMaxY);
		return value;
	}

	float GetScale()
	{
		ImGuiStorage* storage = ImGui::GetStateStorage();
		return storage->GetFloat(ID_Scale) * scaleMultiplier;
	}

	ImVec2 ToRangeHalf(const ImRect& frame_bb, const float spacing)
	{
		ImVec2 count = (frame_bb.Max - frame_bb.Min) / scaleMultiplier;
		ImVec2 range_half;
		range_half.x = spacing * count.x * 0.5f;
		range_half.y = spacing * count.y * 0.5f;
		return range_half;
	}

	ImVec2 ToLocal(const Vector2f& value, const ImRect& inner_bb, const ImRect& range_bb)
	{
		const ImVec2 v = ImVec2(value.x, value.y);
		const ImVec2 t = Remap(v, range_bb.Min, range_bb.Max, ImVec2(0, 1), ImVec2(1, 0));
		return Remap(t, ImVec2(0, 0), ImVec2(1, 1), inner_bb.Min, inner_bb.Max);
	}

	Vector2f ToWorld(const ImVec2& value, const ImRect& inner_bb, const ImRect& range_bb)
	{
		const Vector2f v = Vector2f(value.x, value.y);
		const Vector2f t = Remap(v, inner_bb.Min, inner_bb.Max, Vector2f::Zero, Vector2f::One);
		return Remap(t, Vector2f(0, 1), Vector2f(1, 0), range_bb.Min, range_bb.Max);
	}
}

bool imgui::BeginGraph(const char* label, Vector2f graph_size, ImGuiGraphFlags flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* parent_window = ImGui::GetCurrentWindow();
	if (parent_window->SkipItems)
		return false;

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
		return false;
	}

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiStorage* storage = window->DC.StateStorage;
	if (window->SkipItems)
		return false;

	float scale = 1.f;
	float spacing = 1.f;
	ImVec2 position = ImVec2(0.f, 0.f);
	scale = storage->GetFloat(ID_Scale, scale);
	spacing = storage->GetFloat(ID_Spacing, spacing);
	position.x = storage->GetFloat(ID_PositionX, position.x);
	position.y = storage->GetFloat(ID_PositionY, position.y);

	if (ImGui::IsMouseDragging(ImGuiMouseButton_Right) && ImGui::ItemHoverable(frame_bb, id))
	{
		const ImVec2 delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
		ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);
		position.x -= delta.x * spacing * 0.01f;
		position.y += delta.y * spacing * 0.01f;
	}

	if (ImGui::GetIO().MouseWheel != 0 && ImGui::ItemHoverable(frame_bb, id))
	{
		constexpr float count = 18.f;
		const float min = std::powf(0.5f, count);
		const float max = std::powf(2.0f, count);

		scale *= math::Remap(ImGui::GetIO().MouseWheel, -1.f, 1.f, 0.5f, 2.f);
		scale = math::Clamp(scale, min, max);

		spacing *= math::Remap(ImGui::GetIO().MouseWheel, -1.f, 1.f, 2.f, 0.5f);
		spacing = math::Clamp(spacing, min, max);
	}

	const ImVec2 range_half = ToRangeHalf(frame_bb, spacing);

	ImRect range_bb;
	range_bb.Min = position - range_half;
	range_bb.Max = position + range_half;

	storage->SetFloat(ID_Scale, scale);
	storage->SetFloat(ID_Spacing, spacing);
	storage->SetFloat(ID_PositionX, position.x);
	storage->SetFloat(ID_PositionY, position.y);
	storage->SetFloat(ID_FrameMinX, frame_bb.Min.x);
	storage->SetFloat(ID_FrameMinY, frame_bb.Min.y);
	storage->SetFloat(ID_FrameMaxX, frame_bb.Max.x);
	storage->SetFloat(ID_FrameMaxY, frame_bb.Max.y);
	storage->SetFloat(ID_InnerMinX, inner_bb.Min.x);
	storage->SetFloat(ID_InnerMinY, inner_bb.Min.y);
	storage->SetFloat(ID_InnerMaxX, inner_bb.Max.x);
	storage->SetFloat(ID_InnerMaxY, inner_bb.Max.y);
	storage->SetFloat(ID_RangeMinX, range_bb.Min.x);
	storage->SetFloat(ID_RangeMinY, range_bb.Min.y);
	storage->SetFloat(ID_RangeMaxX, range_bb.Max.x);
	storage->SetFloat(ID_RangeMaxY, range_bb.Max.y);

	if (flags & ImGuiGraphFlags_Grid)
	{
		const ImU32 col_major = ImGui::GetColorU32(ImGuiCol_SeparatorActive);
		const ImU32 col_minor = ImGui::GetColorU32(ImGuiCol_Separator);

		ImVec2 pos0 = frame_bb.Min, pos1 = frame_bb.Min;
		ImVec2 clamp_min = { math::Round(range_bb.Min.x, spacing), math::Round(range_bb.Min.y, spacing) };
		ImVec2 clamp_max = { math::Round(range_bb.Max.x, spacing), math::Round(range_bb.Max.y, spacing) };

		// draw vertical lines
		pos0.y = frame_bb.Min.y;
		pos1.y = frame_bb.Max.y;
		for (float x = clamp_min.x; x <= clamp_max.x; x += spacing)
		{
			const bool isMajor = (math::Round<int32>(x / spacing) % grid_major) == 0;
			const ImU32 col = isMajor ? col_major : col_minor;

			pos0.x = math::Remap(x, range_bb.Min.x, range_bb.Max.x, frame_bb.Min.x, frame_bb.Max.x);
			pos1.x = math::Remap(x, range_bb.Min.x, range_bb.Max.x, frame_bb.Min.x, frame_bb.Max.x);
			window->DrawList->AddLine(pos0, pos1, col);

			if (isMajor && flags & ImGuiGraphFlags_TextX)
			{
				char buff0[8];
				const char* buff1 = buff0 + ImGui::DataTypeFormatString(buff0, IM_ARRAYSIZE(buff0), ImGuiDataType_Float, &x, " %g");
				ImGui::RenderTextClipped(pos0, frame_bb.Max, buff0, buff1, nullptr);
			}
		}

		// draw horizontal lines
		pos0.x = frame_bb.Min.x;
		pos1.x = frame_bb.Max.x;
		for (float y = clamp_min.y; y <= clamp_max.y; y += spacing)
		{
			const bool isMajor = (math::Round<int32>(y / spacing) % grid_major) == 0;
			const ImU32 col = isMajor ? col_major : col_minor;

			pos0.y = math::Remap(y, range_bb.Min.y, range_bb.Max.y, frame_bb.Max.y, frame_bb.Min.y);
			pos1.y = math::Remap(y, range_bb.Min.y, range_bb.Max.y, frame_bb.Max.y, frame_bb.Min.y);
			window->DrawList->AddLine(pos0, pos1, col);

			if (isMajor && flags & ImGuiGraphFlags_TextX)
			{
				char buff0[8];
				const char* buff1 = buff0 + ImGui::DataTypeFormatString(buff0, IM_ARRAYSIZE(buff0), ImGuiDataType_Float, &y, " %g");
				ImGui::RenderTextClipped(pos0, frame_bb.Max, buff0, buff1, nullptr);
			}
		}
	}

	return true;
}

void imgui::EndGraph()
{
	ImGuiStorage* storage = ImGui::GetStateStorage();
	storage->SetInt(ID_ShapeIdx, 0);

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		storage->SetInt(ID_DragIdx, 0);

	ImGui::EndChildFrame();
}

void imgui::GraphShape(Circle2f& value, int segments, float thickness)
{
	const float scale = GetScale();
	const int dragIdx = GenerateDragIndex();
	const ImU32 colourA = ImGui::GetColorU32(ImGuiCol_PlotLines);
	const ImU32 colourB = ImGui::GetColorU32(ImGuiCol_PlotLinesHovered);
	const ImRect inner_bb = GetInner();
	const ImRect range_bb = GetRange();
	const ImVec2 mousePos = ImGui::GetIO().MousePos;

	ImVec2 localPos = ToLocal(value.m_Position, inner_bb, range_bb);
	const bool isHovered = IsOverlappingControl(mousePos, localPos);
	if (isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		AssignDrag(dragIdx);

	if (IsDragging(ImGuiMouseButton_Left, dragIdx))
		localPos = mousePos;

	float radius = value.m_Radius * scale;
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddCircle(localPos, radius, colourA, segments, thickness);

	const ImU32 colour = !isHovered ? colourA : colourB;
	drawList->AddCircle(localPos, controlRadius, colour);

	value.m_Position = ToWorld(localPos, inner_bb, range_bb);
	value.m_Radius = radius / scale;
}

void imgui::GraphShape(Line2f& value, float thickness)
{
	const float scale = GetScale();
	const int dragIdx1 = GenerateDragIndex();
	const int dragIdx2 = GenerateDragIndex();
	const ImU32 colourA = ImGui::GetColorU32(ImGuiCol_PlotLines);
	const ImU32 colourB = ImGui::GetColorU32(ImGuiCol_PlotLinesHovered);
	const ImRect inner_bb = GetInner();
	const ImRect range_bb = GetRange();
	const ImVec2 mousePos = ImGui::GetIO().MousePos;

	ImVec2 p1 = ToLocal(value.m_PointA, inner_bb, range_bb);
	const bool isHovered1 = IsOverlappingControl(mousePos, p1);
	if (isHovered1 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		AssignDrag(dragIdx1);

	ImVec2 p2 = ToLocal(value.m_PointB, inner_bb, range_bb);
	const bool isHovered2 = IsOverlappingControl(mousePos, p2);
	if (isHovered2 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		AssignDrag(dragIdx2);

	if (IsDragging(ImGuiMouseButton_Left, dragIdx1))
		p1 = mousePos;
	if (IsDragging(ImGuiMouseButton_Left, dragIdx2))
		p2 = mousePos;

	ImVec2 d0 = p2 - p1;
	ImVec2 p3 = p1 + d0 * 9999.f;
	ImVec2 p4 = p2 - d0 * 9999.f;
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddLine(p3, p4, colourA, thickness);

	const ImU32 colour1 = !isHovered1 ? colourA : colourB;
	const ImU32 colour2 = !isHovered2 ? colourA : colourB;
	drawList->AddCircle(p1, controlRadius, colour1);
	drawList->AddCircle(p2, controlRadius, colour2);

	value.m_PointA = ToWorld(p1, inner_bb, range_bb);
	value.m_PointB = ToWorld(p2, inner_bb, range_bb);
}

void imgui::GraphShape(Ray2f& value, float thickness)
{
	const float scale = GetScale();
	const int dragIdx1 = GenerateDragIndex();
	const int dragIdx2 = GenerateDragIndex();
	const ImU32 colourA = ImGui::GetColorU32(ImGuiCol_PlotLines);
	const ImU32 colourB = ImGui::GetColorU32(ImGuiCol_PlotLinesHovered);
	const ImRect inner_bb = GetInner();
	const ImRect range_bb = GetRange();
	const ImVec2 mousePos = ImGui::GetIO().MousePos;

	ImVec2 p1 = ToLocal(value.m_Position, inner_bb, range_bb);
	const bool isHovered1 = IsOverlappingControl(mousePos, p1);
	if (isHovered1 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		AssignDrag(dragIdx1);

	ImVec2 d1 = { value.m_Direction.x, -value.m_Direction.y };
	ImVec2 p2 = p1 + d1 * scaleMultiplier;
	const bool isHovered2 = IsOverlappingControl(mousePos, p2);
	if (isHovered2 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		AssignDrag(dragIdx2);

	if (IsDragging(ImGuiMouseButton_Left, dragIdx1))
		p1 = mousePos;

	if (IsDragging(ImGuiMouseButton_Left, dragIdx2))
	{
		p2 = mousePos;
		d1 = p2 - p1;
	}

	ImVec2 p3 = p1 + d1 * 9999.f;
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddLine(p1, p3, colourA, thickness);

	const ImU32 colour1 = !isHovered1 ? colourA : colourB;
	const ImU32 colour2 = !isHovered2 ? colourA : colourB;
	drawList->AddCircle(p1, controlRadius, colour1);
	drawList->AddCircle(p2, controlRadius, colour2);

	value.m_Position = ToWorld(p1, inner_bb, range_bb);
	value.m_Direction = Vector2f(d1.x, -d1.y);
	value.m_Direction.Normalize();
}

void imgui::GraphShape(AABB2f& value, float rounding, float thickness, ImDrawFlags flags)
{
	const float scale = GetScale();
	const int dragIdx1 = GenerateDragIndex();
	const int dragIdx2 = GenerateDragIndex();
	const int dragIdx3 = GenerateDragIndex();
	const int dragIdx4 = GenerateDragIndex();
	const ImU32 colourA = ImGui::GetColorU32(ImGuiCol_PlotLines);
	const ImU32 colourB = ImGui::GetColorU32(ImGuiCol_PlotLinesHovered);
	const ImRect inner_bb = GetInner();
	const ImRect range_bb = GetRange();
	const ImVec2 mousePos = ImGui::GetIO().MousePos;

	ImVec2 p1 = ToLocal(value.m_Min, inner_bb, range_bb);
	ImVec2 p3 = ToLocal(value.m_Max, inner_bb, range_bb);
	ImVec2 p2 = { p1.x, p3.y };
	ImVec2 p4 = { p3.x, p1.y };

	const bool isHovered1 = IsOverlappingControl(mousePos, p1);
	if (isHovered1 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		AssignDrag(dragIdx1);
	const bool isHovered2 = IsOverlappingControl(mousePos, p2);
	if (isHovered2 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		AssignDrag(dragIdx2);
	const bool isHovered3 = IsOverlappingControl(mousePos, p3);
	if (isHovered3 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		AssignDrag(dragIdx3);
	const bool isHovered4 = IsOverlappingControl(mousePos, p4);
	if (isHovered4 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		AssignDrag(dragIdx4);

	if (IsDragging(ImGuiMouseButton_Left, dragIdx1))
	{
		p1 = mousePos;
		p2.x = mousePos.x;
		p4.y = mousePos.y;
	}
	if (IsDragging(ImGuiMouseButton_Left, dragIdx2))
	{
		p1.x = mousePos.x;
		p2 = mousePos;
		p3.y = mousePos.y;
	}
	if (IsDragging(ImGuiMouseButton_Left, dragIdx3))
	{
		p2.y = mousePos.y;
		p3 = mousePos;
		p4.x = mousePos.x;
	}
	if (IsDragging(ImGuiMouseButton_Left, dragIdx4))
	{
		p3.x = mousePos.x;
		p4 = mousePos;
		p1.y = mousePos.y;
	}

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddRect(p1, p3, colourA, rounding, flags, thickness);

	drawList->AddCircle(p1, controlRadius, !isHovered1 ? colourA : colourB);
	drawList->AddCircle(p2, controlRadius, !isHovered2 ? colourA : colourB);
	drawList->AddCircle(p3, controlRadius, !isHovered3 ? colourA : colourB);
	drawList->AddCircle(p4, controlRadius, !isHovered4 ? colourA : colourB);

	value.m_Min = ToWorld(p1, inner_bb, range_bb);
	value.m_Max = ToWorld(p3, inner_bb, range_bb);
	value.m_Min = math::Min(value.m_Min, value.m_Max);
	value.m_Max = math::Max(value.m_Min, value.m_Max);
}

void imgui::GraphShape(Segment2f& value, float thickness)
{
	const float scale = GetScale();
	const int dragIdx1 = GenerateDragIndex();
	const int dragIdx2 = GenerateDragIndex();
	const ImU32 colourA = ImGui::GetColorU32(ImGuiCol_PlotLines);
	const ImU32 colourB = ImGui::GetColorU32(ImGuiCol_PlotLinesHovered);
	const ImRect inner_bb = GetInner();
	const ImRect range_bb = GetRange();
	const ImVec2 mousePos = ImGui::GetIO().MousePos;

	ImVec2 p1 = ToLocal(value.m_PointA, inner_bb, range_bb);
	const bool isHovered1 = IsOverlappingControl(mousePos, p1);
	if (isHovered1 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		AssignDrag(dragIdx1);

	ImVec2 p2 = ToLocal(value.m_PointB, inner_bb, range_bb);
	const bool isHovered2 = IsOverlappingControl(mousePos, p2);
	if (isHovered2 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		AssignDrag(dragIdx2);

	if (IsDragging(ImGuiMouseButton_Left, dragIdx1))
		p1 = mousePos;
	if (IsDragging(ImGuiMouseButton_Left, dragIdx2))
		p2 = mousePos;

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddLine(p1, p2, colourA, thickness);

	const ImU32 colour1 = !isHovered1 ? colourA : colourB;
	const ImU32 colour2 = !isHovered2 ? colourA : colourB;
	drawList->AddCircle(p1, controlRadius, colour1);
	drawList->AddCircle(p2, controlRadius, colour2);

	value.m_PointA = ToWorld(p1, inner_bb, range_bb);
	value.m_PointB = ToWorld(p2, inner_bb, range_bb);
}

void imgui::GraphShape(Triangle2f& value, float thickness)
{
	const float scale = GetScale();
	const int dragIdx1 = GenerateDragIndex();
	const int dragIdx2 = GenerateDragIndex();
	const int dragIdx3 = GenerateDragIndex();
	const ImU32 colourA = ImGui::GetColorU32(ImGuiCol_PlotLines);
	const ImU32 colourB = ImGui::GetColorU32(ImGuiCol_PlotLinesHovered);
	const ImRect inner_bb = GetInner();
	const ImRect range_bb = GetRange();
	const ImVec2 mousePos = ImGui::GetIO().MousePos;

	ImVec2 p1 = ToLocal(value.m_PointA, inner_bb, range_bb);
	const bool isHovered1 = IsOverlappingControl(mousePos, p1);
	if (isHovered1 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		AssignDrag(dragIdx1);

	ImVec2 p2 = ToLocal(value.m_PointB, inner_bb, range_bb);
	const bool isHovered2 = IsOverlappingControl(mousePos, p2);
	if (isHovered2 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		AssignDrag(dragIdx2);

	ImVec2 p3 = ToLocal(value.m_PointC, inner_bb, range_bb);
	const bool isHovered3 = IsOverlappingControl(mousePos, p3);
	if (isHovered3 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		AssignDrag(dragIdx3);

	if (IsDragging(ImGuiMouseButton_Left, dragIdx1))
		p1 = mousePos;
	if (IsDragging(ImGuiMouseButton_Left, dragIdx2))
		p2 = mousePos;
	if (IsDragging(ImGuiMouseButton_Left, dragIdx3))
		p3 = mousePos;

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddTriangle(p1, p2, p3, colourA, thickness);

	const ImU32 colour1 = !isHovered1 ? colourA : colourB;
	const ImU32 colour2 = !isHovered2 ? colourA : colourB;
	const ImU32 colour3 = !isHovered3 ? colourA : colourB;
	drawList->AddCircle(p1, controlRadius, colour1);
	drawList->AddCircle(p2, controlRadius, colour2);
	drawList->AddCircle(p3, controlRadius, colour3);

	value.m_PointA = ToWorld(p1, inner_bb, range_bb);
	value.m_PointB = ToWorld(p2, inner_bb, range_bb);
	value.m_PointC = ToWorld(p3, inner_bb, range_bb);
}

void imgui::GraphShape(Vector2f& value, float thickness)
{
	const float scale = GetScale();
	const int dragIdx = GenerateDragIndex();
	const ImU32 colourA = ImGui::GetColorU32(ImGuiCol_PlotLines);
	const ImU32 colourB = ImGui::GetColorU32(ImGuiCol_PlotLinesHovered);
	const ImRect inner_bb = GetInner();
	const ImRect range_bb = GetRange();
	const ImVec2 mousePos = ImGui::GetIO().MousePos;

	ImVec2 localPos = ToLocal(value, inner_bb, range_bb);
	const bool isHovered = IsOverlappingControl(mousePos, localPos);
	if (isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		AssignDrag(dragIdx);

	if (IsDragging(ImGuiMouseButton_Left, dragIdx))
		localPos = mousePos;

	const ImU32 colour = !isHovered ? colourA : colourB;
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddCircle(localPos, controlRadius, colour);

	value = ToWorld(localPos, inner_bb, range_bb);
}