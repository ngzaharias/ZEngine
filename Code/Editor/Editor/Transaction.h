#pragma once

#include <Core/Address.h>
#include <Core/String.h>

#include <Engine/JsonTypes.h>

class Vector2f;
class Vector2i;
class Vector2u;
class Vector3f;
class Vector3i;

namespace editor
{
	struct Transaction
	{
		enum class EType { Added, Edited, Removed, };

		template<typename Type>
		Transaction(const str::Address& address, const Type& value)
			: Transaction(address.GetFull(), value)
		{
		}

		template<typename Type>
		Transaction(str::StringView address, const Type& value)
		{
			//static_assert(false, "Unsupported Type!");
		}

		Transaction(str::StringView address, const bool& value);
		Transaction(str::StringView address, const float& value);
		Transaction(str::StringView address, const int32& value);
		Transaction(str::StringView address, const uint32& value);
		Transaction(str::StringView address, const Vector2f& value);
		Transaction(str::StringView address, const Vector2i& value);
		Transaction(str::StringView address, const Vector2u& value);
		Transaction(str::StringView address, const Vector3f& value);
		Transaction(str::StringView address, const Vector3i& value);
		Transaction(str::StringView address, const str::String& value);
		Transaction(str::StringView address, const json::Object& value);

		void ApplyTo(json::Document& document) const;

	public:
		str::String		m_Address = { };
		json::Document	m_Document = { };
		EType			m_Type = EType::Edited;
	};
}