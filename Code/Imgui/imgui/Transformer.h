#pragma once

class Matrix4x4;

namespace imgui
{
	bool Translate2D(Matrix4x4& model, const Matrix4x4& view, const Matrix4x4& projection);
}
