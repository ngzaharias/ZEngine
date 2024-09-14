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
	public:
		Inspector();
		~Inspector();

	public:
		// Read
		template<typename Value>
		void Read(const char* label, const Value& value);
		void Read(const char* label, const bool& value);
		void Read(const char* label, const float& value);
		void Read(const char* label, const double& value);
		void Read(const char* label, const int8& value);
		void Read(const char* label, const int16& value);
		void Read(const char* label, const int32& value);
		void Read(const char* label, const int64& value);
		void Read(const char* label, const uint8& value);
		void Read(const char* label, const uint16& value);
		void Read(const char* label, const uint32& value);
		void Read(const char* label, const uint64& value);
		void Read(const char* label, const str::Guid& value);
		void Read(const char* label, const str::Name& value);
		void Read(const char* label, const str::Path& value);
		void Read(const char* label, const str::String& value);

		// Write
		template<typename Value>
		bool Write(const char* label, Value& value);
		bool Write(const char* label, bool& value);
		bool Write(const char* label, float& value);
		bool Write(const char* label, double& value);
		bool Write(const char* label, int8& value);
		bool Write(const char* label, int16& value);
		bool Write(const char* label, int32& value);
		bool Write(const char* label, int64& value);
		bool Write(const char* label, uint8& value);
		bool Write(const char* label, uint16& value);
		bool Write(const char* label, uint32& value);
		bool Write(const char* label, uint64& value);
		bool Write(const char* label, str::Guid& value);
		bool Write(const char* label, str::Name& value);
		bool Write(const char* label, str::Path& value);
		bool Write(const char* label, str::String& value);

	private:
		// Read
		template<typename Value>
		void ReadArray(const Array<Value>& values);

		template<typename Value>
		inline void ReadCustom(const Value& value);

		template <typename TEnum>
		void ReadEnum(const TEnum& value);

		template<typename Value>
		void ReadMap(const Map<str::Guid, Value>& values);
		template<typename Value>
		void ReadMap(const Map<str::Name, Value>& values);
		template<typename Value>
		void ReadMap(const Map<str::String, Value>& values);

		template<typename Type>
		void ReadOptional(const Optional<Type>& value);

		template<typename ...Types>
		void ReadVariant(const Variant<Types...>& value);

		// Write
		template<typename Value>
		bool WriteArray(Array<Value>& values);

		template<typename Value>
		inline bool WriteCustom(Value& value);

		template <typename TEnum>
		bool WriteEnum(TEnum& value);

		template<typename Value>
		bool WriteMap(Map<str::Guid, Value>& values);
		template<typename Value>
		bool WriteMap(Map<str::Name, Value>& values);
		template<typename Value>
		bool WriteMap(Map<str::String, Value>& values);

		template<typename Type>
		bool WriteOptional(Optional<Type>& value);

		template<typename ...Types>
		bool WriteVariant(Variant<Types...>& value);

	private:
		bool m_IsVisible = false;
	};
}

#include "Inspector.inl"