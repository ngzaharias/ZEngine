#pragma once

#include <Core/Guid.h>
#include <Core/String.h>
#include <Core/Variant.h>
#include <Core/Vector.h>

#include <Engine/Name.h>

namespace node
{
	using Var = Variant<
		bool,
		float,
		int32,
		Vector2f,
		Vector3f,
		Vector4f,
		str::Guid,
		str::Name,
		str::String>;
}