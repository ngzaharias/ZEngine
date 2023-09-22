#pragma once

#include <Core/Quaternion.h>
#include <Core/Rotator.h>
#include <Core/String.h>

class Vector2f;
class Vector2i;
class Vector2u;
class Vector3f;
class Vector3i;

namespace editor
{
	void TypeOverload(bool& value);
	void TypeOverload(float& value);
	void TypeOverload(int32& value);
	void TypeOverload(uint32& value);
	void TypeOverload(Quaternion& value);
	void TypeOverload(Rotator& value);
	void TypeOverload(Vector2f& value);
	void TypeOverload(Vector2i& value);
	void TypeOverload(Vector2u& value);
	void TypeOverload(Vector3f& value);
	void TypeOverload(Vector3i& value);
	void TypeOverload(str::String& value);
}
