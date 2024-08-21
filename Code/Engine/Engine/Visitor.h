#pragma once

#include "Core/Variant.h"
#include "Engine/TomlTypes.h"

#include <iostream>

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
	using String = std::string;
	using StringView = std::string_view;
}

namespace eng
{
	class Visitor
	{
	public:
		friend std::ostream& operator<<(std::ostream& lhs, const eng::Visitor& rhs);

	public:
		struct Iterator
		{
			using Itr = toml::Table::iterator;
			using Node = toml::Node;

			Iterator(Itr itr, Node** node);
			~Iterator();

			auto operator*()->str::StringView;
			auto operator++()->Iterator&;
			bool operator!=(const Iterator& rhs) const;

		private:
			Itr m_Itr;
			Node** m_Node;
			Node* m_Parent;
		};

	public:
		Visitor() noexcept = default;
		Visitor(Visitor&& rhs) noexcept;
		Visitor(const str::StringView& value);

		Visitor& operator=(Visitor&& rhs) noexcept;
		Visitor& operator=(const str::Path& rhs) noexcept;
		Visitor& operator=(const str::StringView& rhs) noexcept;

		operator str::String() noexcept;

		/// \brief Marks that the current object being visited should be streamed as inline.
		void SetInline();

		/// \brief Save the contents of the visitor to file.
		/// Can be done regardless if the file is in read/write mode.
		bool SaveToFile(const str::Path& path);

		/// \brief Read the contents of a file to the visitor.
		/// Also switches the visitor to read mode.
		bool LoadFromFile(const str::Path& path);

	public:
		// Read
		template<typename Value>
		void Read(Value& value) const;

		template<typename Key, typename Value>
		void Read(Map<Key, Value>& values) const;

		template<typename Value>
		void Read(const str::StringView& key, Value& value, const Value& defaultValue) const;
		void Read(const str::StringView& key, bool& value, const bool defaultValue) const;
		void Read(const str::StringView& key, float& value, const float defaultValue) const;
		void Read(const str::StringView& key, double& value, const double defaultValue) const;
		void Read(const str::StringView& key, int8& value, const int8 defaultValue) const;
		void Read(const str::StringView& key, int16& value, const int16 defaultValue) const;
		void Read(const str::StringView& key, int32& value, const int32 defaultValue) const;
		void Read(const str::StringView& key, int64& value, const int64 defaultValue) const;
		void Read(const str::StringView& key, uint8& value, const uint8 defaultValue) const;
		void Read(const str::StringView& key, uint16& value, const uint16 defaultValue) const;
		void Read(const str::StringView& key, uint32& value, const uint32 defaultValue) const;
		void Read(const str::StringView& key, str::Guid& value, const str::Guid& defaultValue) const;
		void Read(const str::StringView& key, str::Name& value, const str::Name& defaultValue) const;
		void Read(const str::StringView& key, str::Path& value, const str::Path& defaultValue) const;
		void Read(const str::StringView& key, str::String& value, const str::String& defaultValue) const;

		template<typename Value>
		void Read(const int32 index, Value& value) const;
		void Read(const int32 index, bool& value) const;
		void Read(const int32 index, float& value) const;
		void Read(const int32 index, double& value) const;
		void Read(const int32 index, int8& value) const;
		void Read(const int32 index, int16& value) const;
		void Read(const int32 index, int32& value) const;
		void Read(const int32 index, int64& value) const;
		void Read(const int32 index, uint8& value) const;
		void Read(const int32 index, uint16& value) const;
		void Read(const int32 index, uint32& value) const;
		void Read(const int32 index, str::Guid& value) const;
		void Read(const int32 index, str::Name& value) const;
		void Read(const int32 index, str::Path& value) const;
		void Read(const int32 index, str::String& value) const;

		// Write
		template<typename Value>
		void Write(const Value& value);

		template<typename Key, typename Value>
		void Write(const Map<Key, Value>& values);

		template<typename Value>
		void Write(const str::StringView& key, const Value& value);
		void Write(const str::StringView& key, const bool& value);
		void Write(const str::StringView& key, const float& value);
		void Write(const str::StringView& key, const double& value);
		void Write(const str::StringView& key, const int8& value);
		void Write(const str::StringView& key, const int16& value);
		void Write(const str::StringView& key, const int32& value);
		void Write(const str::StringView& key, const int64& value);
		void Write(const str::StringView& key, const uint8& value);
		void Write(const str::StringView& key, const uint16& value);
		void Write(const str::StringView& key, const uint32& value);
		void Write(const str::StringView& key, const str::Guid& value);
		void Write(const str::StringView& key, const str::Name& value);
		void Write(const str::StringView& key, const str::Path& value);
		void Write(const str::StringView& key, const str::String& value);

		template<typename Value>
		void Write(const int32 index, const Value& value);
		void Write(const int32 index, const bool& value);
		void Write(const int32 index, const float& value);
		void Write(const int32 index, const double& value);
		void Write(const int32 index, const int8& value);
		void Write(const int32 index, const int16& value);
		void Write(const int32 index, const int32& value);
		void Write(const int32 index, const int64& value);
		void Write(const int32 index, const uint8& value);
		void Write(const int32 index, const uint16& value);
		void Write(const int32 index, const uint32& value);
		void Write(const int32 index, const str::Guid& value);
		void Write(const int32 index, const str::Name& value);
		void Write(const int32 index, const str::Path& value);
		void Write(const int32 index, const str::String& value);

	private:
		// Read
		template<typename Value>
		void ReadArray(Array<Value>& values) const;

		template<typename Value>
		inline void ReadCustom(Value& value) const;

		template <typename TEnum>
		void ReadEnum(const str::StringView& key, TEnum& value, const TEnum defaultValue) const;
		template <typename TEnum>
		void ReadEnum(const int32 index, TEnum& value) const;

		template<typename Value>
		void ReadMap(Map<str::Guid, Value>& values) const;
		template<typename Value>
		void ReadMap(Map<str::Name, Value>& values) const;
		template<typename Value>
		void ReadMap(Map<str::String, Value>& values) const;

		template<typename Value>
		void ReadPrimitive(const str::StringView& key, Value& value, const Value defaultValue) const;
		template<typename Value>
		void ReadPrimitive(const int32 index, Value& value) const;

		template<typename ...Types>
		void ReadVariant(Variant<Types...>& value) const;

		// Write
		template<typename Value>
		void WriteArray(const Array<Value>& values);

		template<typename Value>
		inline void WriteCustom(const Value& value);

		template <typename TEnum>
		void WriteEnum(const str::StringView& key, const TEnum& value);
		template <typename TEnum>
		void WriteEnum(const int32 index, const TEnum& value);

		template<typename Value>
		void WriteMap(const Map<str::Guid, Value>& values);
		template<typename Value>
		void WriteMap(const Map<str::Name, Value>& values);
		template<typename Value>
		void WriteMap(const Map<str::String, Value>& values);

		template<typename Value>
		void WritePrimitive(const str::StringView& key, const Value& value);
		template<typename Value>
		void WritePrimitive(const int32 index, const Value& value);

		template<typename ...Types>
		void WriteVariant(const Variant<Types...>& value);

	public:
		auto begin()->Iterator;
		auto end()->Iterator;

	public:
		toml::Table m_Root = { };

	private:
		mutable toml::Node* m_Node = &m_Root;
	};
}

#include "Visitor.inl"