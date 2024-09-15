#pragma once

#include "Core/Optional.h"
#include "Core/String.h"
#include "Core/StringView.h"
#include "Core/Variant.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

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
	class Inspector
	{
		template<typename Value>
		struct IsCollapsable : std::true_type {};

	public:
		bool Begin(const char* label);
		void End();

		template<typename Value>
		void Read(const char* label, const Value& value);
		template<typename Value>
		bool Write(const char* label, Value& value);

	private:
		//////////////////////////////////////////////////////////////////////////
		// Read
		template<typename Value>
		void ReadArray(const Array<Value>& values);

		template<typename Value>
		inline void ReadCustom(const Value& value);
		template<typename Value>
		inline void ReadInline(const Value& value);

		template <typename Value>
		void ReadEnum(const Value& value);

		template<typename Key, typename Value>
		void ReadMap(const Map<Key, Value>& values);

		template<typename Value>
		void ReadOptional(const Optional<Value>& value);

		template<typename ...Values>
		void ReadVariant(const Variant<Values...>& value);

		//////////////////////////////////////////////////////////////////////////
		// Write
		template<typename Value>
		bool WriteArray(Array<Value>& values);

		template<typename Value>
		inline bool WriteCustom(Value& value);
		template<typename Value>
		inline bool WriteInline(Value& value);

		template <typename TEnum>
		bool WriteEnum(TEnum& value);

		template<typename Key, typename Value>
		bool WriteMap(Map<Key, Value>& values);

		template<typename Value>
		bool WriteOptional(Optional<Value>& value);

		template<typename ...Values>
		bool WriteVariant(Variant<Values...>& value);
	};
}

#include "Inspector.inl"