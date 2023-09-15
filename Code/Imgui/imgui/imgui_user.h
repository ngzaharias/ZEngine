#pragma once

struct ImVec2;
struct ImVec4;
typedef unsigned int ImU32;

enum ImGuiGraphFlags_
{
	ImGuiGraphFlags_Grid = 1 << 0,	// No condition (always set the variable)
};

using ImGuiGraphFlags = int; // -> enum ImGuiGraphFlags_     // Flags: for Begin(), BeginChild()

namespace imgui
{
	void Bullet();
	bool BulletHeader(const char* label, const bool selected = false);

	void PlotLines(const char* label, float* values, int values_count, ImVec2 graph_size, ImGuiGraphFlags flags = 0);
	void PlotLines(const char* label, ImVec2* values, int values_count, ImVec2 graph_size, ImGuiGraphFlags flags = 0);

	/// \brief Converts a hexadecimal colour to an ImVec4 colour.
	/// Example: 0xFF00FF00 -> { 1.f, 0.f, 1.f, 0.f }
	ImVec4 ToColour(const int hexadecimal);

	/// \brief Converts a RGBA hexadecimal colour to a ImU32 colour.
	ImU32 ToColour32(const int hexadecimal);
}