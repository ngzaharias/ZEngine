#pragma once

#include "Core/Optional.h"
#include "Core/SparseArray.h"
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
		void Read(const char* label, const Value& value);
		template<typename Value>
		bool Write(const char* label, Value& value);

	public:
		template<class TPayload>
		void AddPayload(TPayload& resource);

		template<class TPayload>
		bool HasPayload();

		template<class TPayload>
		auto GetPayload()->TPayload&;

	protected:
		template<typename Value>
		inline void ReadCustom(const Value& value);
		template<typename Value>
		inline bool WriteCustom(Value& value);

		template<typename Value>
		bool ReadHeader(const char* label, const Value& value);
		template<typename Value>
		bool WriteHeader(const char* label, Value& value);

		template<typename Value>
		void ReadMember(const Value& value);
		template<typename Value>
		bool WriteMember(Value& value);

	protected:
		template<typename Value>
		void ReadArray(const Array<Value>& values);
		template<typename Value>
		bool WriteArray(Array<Value>& values);

		template <typename Value>
		void ReadEnum(const Value& value);
		template <typename TEnum>
		bool WriteEnum(TEnum& value);

		template<typename Key, typename Value>
		void ReadMap(const Map<Key, Value>& values);
		template<typename Key, typename Value>
		bool WriteMap(Map<Key, Value>& values);

		template<typename Value>
		void ReadOptional(const Optional<Value>& value);
		template<typename Value>
		bool WriteOptional(Optional<Value>& value);

		template<typename Value>
		void ReadSet(const Set<Value>& values);
		template<typename Value>
		bool WriteSet(Set<Value>& value);

		template<typename ...Values>
		void ReadVariant(const Variant<Values...>& value);
		template<typename ...Values>
		bool WriteVariant(Variant<Values...>& value);

	protected:
		SparseArray<int32, void*> m_Payload = { };
	};
}

#include "Inspector.inl"