#pragma once

#include "Core/Colour.h"
#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/String.h"
#include "Engine/TableManager.h"

namespace eng
{
	struct ColourObject
	{
		str::Name m_Name = {};
		::Colour m_Colour = {};
	};

	class ColourTable final : public eng::TableManager<str::Guid, eng::ColourObject>
	{
	public:
		const eng::ColourObject& GetColour(const str::Guid& value) const;
	};
}