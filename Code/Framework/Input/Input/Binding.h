#pragma once

#include "Core/Name.h"
#include "Input/Key.h"

namespace input
{
	struct Binding
	{
		Binding() = default;
		Binding(const str::Name& name, const input::EKey& key)
			: m_Name(name), m_Primary(key) { }
		Binding(const str::Name& name, const input::EKey& key, const bool consume)
			: m_Name(name), m_Primary(key), m_Consume(consume) { }
		Binding(const str::Name& name, const input::EKey& primary, const input::EKey& modifier)
			: m_Name(name), m_Primary(primary), m_Modifier(modifier) { }
		Binding(const str::Name& name, const input::EKey& primary, const input::EKey& modifier, const bool consume)
			: m_Name(name), m_Primary(primary), m_Modifier(modifier), m_Consume(consume) { }

		str::Name m_Name = {};
		bool m_Consume = true;

		input::EKey m_Primary = EKey::None;
		input::EKey m_Modifier = EKey::None;
	};
}