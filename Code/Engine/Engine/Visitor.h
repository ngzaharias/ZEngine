#pragma once

#include <Core/Variant.h>
#include <Engine/TomlTypes.h>

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
		enum EMode
		{
			Read,
			Write,
		};

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

		/// \brief Returns true if the document is current in read mode.
		bool IsReading() const { return m_Mode == EMode::Read; }
		/// \brief Returns true if the document is current in write mode.
		bool IsWriting() const { return m_Mode == EMode::Write; }

		/// \brief Marks that the current object being visited should be streamed as inline.
		void SetInline();

		/// \brief Swap the document between read/write mode.
		/// See ::Visit for more details.
		void SetMode(const EMode value) { m_Mode = value; };

		/// \brief Save the contents of the visitor to file.
		/// Can be done regardless if the file is in read/write mode.
		bool SaveToFile(const str::Path& path);
		/// \brief Read the contents of a file to the visitor.
		/// Also switches the visitor to read mode.
		bool LoadFromFile(const str::Path& path);

		// Visit as Map
		template<typename Key, typename Value>
		void Visit(Map<Key, Value>& values);

		// Visit by Key
		template<typename Value>
		void Visit(const str::StringView& key, Value& value, const Value& defaultValue);

		void Visit(const str::StringView& key, bool& value, const bool defaultValue);
		void Visit(const str::StringView& key, float& value, const float defaultValue);
		void Visit(const str::StringView& key, double& value, const double defaultValue);
		void Visit(const str::StringView& key, int8& value, const int8 defaultValue);
		void Visit(const str::StringView& key, int16& value, const int16 defaultValue);
		void Visit(const str::StringView& key, int32& value, const int32 defaultValue);
		void Visit(const str::StringView& key, int64& value, const int64 defaultValue);
		void Visit(const str::StringView& key, uint8& value, const uint8 defaultValue);
		void Visit(const str::StringView& key, uint16& value, const uint16 defaultValue);
		void Visit(const str::StringView& key, uint32& value, const uint32 defaultValue);

		void Visit(const str::StringView& key, str::Guid& value, const str::Guid& defaultValue);
		void Visit(const str::StringView& key, str::Name& value, const str::Name& defaultValue);
		void Visit(const str::StringView& key, str::Path& value, const str::Path& defaultValue);
		void Visit(const str::StringView& key, str::String& value, const str::String& defaultValue);

	private:
		// Visit by Index
		template<typename Value>
		void Visit(const int32 index, Value& value);

		void Visit(const int32 index, bool& value);
		void Visit(const int32 index, float& value);
		void Visit(const int32 index, double& value);
		void Visit(const int32 index, int8& value);
		void Visit(const int32 index, int16& value);
		void Visit(const int32 index, int32& value);
		void Visit(const int32 index, int64& value);
		void Visit(const int32 index, uint8& value);
		void Visit(const int32 index, uint16& value);
		void Visit(const int32 index, uint32& value);

		void Visit(const int32 index, str::Guid& value);
		void Visit(const int32 index, str::Name& value);
		void Visit(const int32 index, str::Path& value);
		void Visit(const int32 index, str::String& value);

		template<typename Value>
		void VisitArray(Array<Value>& values);

		template<typename Value>
		inline void VisitCustom(Value& value);

		template <typename TEnum>
		void VisitEnum(const str::StringView& key, TEnum& value, const TEnum defaultValue);
		template <typename TEnum>
		void VisitEnum(const int32 index, TEnum& value);

		template<typename Value>
		void VisitMap(Map<str::Guid, Value>& values);
		template<typename Value>
		void VisitMap(Map<str::Name, Value>& values);
		template<typename Value>
		void VisitMap(Map<str::String, Value>& values);

		template<typename Value>
		void VisitPrimitive(const str::StringView& key, Value& value, const Value defaultValue);
		template<typename Value>
		void VisitPrimitive(const int32 index, Value& value);

		template<typename ...Types>
		void VisitVariant(Variant<Types...>& value);

	public:
		auto begin()->Iterator;
		auto end()->Iterator;

	public:
		toml::Table m_Root = { };

	private:
		toml::Node* m_Node = &m_Root;
		EMode m_Mode = EMode::Write;
	};
}

#include "Visitor.inl"