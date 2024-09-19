#pragma once

#include "Core/Types.h"

namespace imgui
{
	enum class EEnum
	{
		First,
		Second,
	};

	struct StructSimple
	{
		bool operator<(const StructSimple& rhs) const
		{
			return m_Int32 < rhs.m_Int32;
		}

		float m_Float = 0.f;
		int32 m_Int32 = 0;
	};

	struct StructRecursive
	{
		bool operator<(const StructRecursive& rhs) const
		{
			return m_Struct < rhs.m_Struct;
		}

		StructSimple m_Struct = {};
	};

	void InspectorDemo(bool& isWindowOpen);
	void InspectorDemo_Array(bool isReading);
	void InspectorDemo_Basic(bool isReading);
	void InspectorDemo_Map(bool isReading);
	void InspectorDemo_Optional(bool isReading);
	void InspectorDemo_Set(bool isReading);
	void InspectorDemo_Struct(bool isReading);
	void InspectorDemo_Variant(bool isReading);
}