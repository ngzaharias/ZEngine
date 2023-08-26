#pragma once

struct ImVec2;
struct ImVec4;
typedef unsigned int ImU32;

namespace imgui
{
	void Bullet();
	bool BulletHeader(const char* label, const bool selected = false);

	/// \brief Converts a hexadecimal colour to an ImVec4 colour.
	/// Example: 0xFF00FF00 -> { 1.f, 0.f, 1.f, 0.f }
	ImVec4 ToColour(const int hexadecimal);

	/// \brief Converts a RGBA hexadecimal colour to a ImU32 colour.
	ImU32 ToColour32(const int hexadecimal);
}