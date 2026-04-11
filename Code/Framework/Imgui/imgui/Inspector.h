#pragma once

#include "Core/Algorithms.h"
#include "Core/EnumHelpers.h"
#include "Core/Optional.h"
#include "Core/SparseArray.h"
#include "Core/String.h"
#include "Core/StringView.h"
#include "Core/TypeInfo.h"
#include "Core/TypeName.h"
#include "Core/Types.h"
#include "Core/TypeTraits.h"
#include "Core/Variant.h"
#include "Core/VariantHelpers.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"
#include "imgui/Property.h"

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

template<typename Value>
class Array;
template<typename Key, typename Value>
class Map;
template<typename Value>
class Set;

class Vector2f;
class Vector2i;
class Vector2u;
class Vector3f;
class Vector3i;
class Vector4f;

namespace str
{
	class Guid;
	class Path;
	class Name;
}

namespace imgui
{
	struct InspectorTag {};

	class Inspector
	{
	public:
		template<typename Value>
		struct IsInline : std::false_type {};

	public:
		bool Begin(const char* label);
		void End();

		template<typename Value>
		void Read(const Value& value);
		template<typename Value>
		void Read(const char* label, const Value& value);

		template<typename Value>
		bool Write(Value& value);
		template<typename Value>
		bool Write(const char* label, Value& value);
	};
}

#include "Inspector.inl"