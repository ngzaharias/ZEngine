#include "imgui/InspectorDemo.h"

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/Map.h"
#include "Core/Name.h"
#include "Core/Optional.h"
#include "Core/Path.h"
#include "Core/Set.h"
#include "Core/String.h"
#include "Core/Variant.h"
#include "Math/Quaternion.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"
#include "imgui/Inspector.h"

using namespace imgui;

#define VISIT(label, value) \
{ \
	if (isReading) { inspector.Read(label, value); } \
	else { inspector.Write(label, value); } \
}

namespace
{
	struct Struct
	{
		bool m_Bool = false;
		float m_Float = 0.f;
		double m_Double = 0.0;
		int8 m_Int8 = 0;
		int16 m_Int16 = 0;
		int32 m_Int32 = 0;
		int64 m_Int64 = 0;
		uint8 m_UInt8 = 0;
		uint16 m_UInt16 = 0;
		uint32 m_UInt32 = 0;
		uint64 m_UInt64 = 0;
		EEnum m_Enum = EEnum::First;

		str::Guid m_Guid = {};
		str::Name m_Name = {};
		str::Path m_Path = {};
		str::String m_String = {};

		Quaternion m_Quaternion = {};
		Rotator m_Rotator = {};
		Vector2f m_Vector2f = {};
		Vector2i m_Vector2i = {};
		Vector2u m_Vector2u = {};
		Vector3f m_Vector3f = {};
		Vector3i m_Vector3i = {};
		Vector4f m_Vector4f = {};
		StructSimple m_StructSimple = {};
		StructRecursive m_StructRecursive = {};
		Optional<int32> m_Optional = { 0 };
		Variant<bool, int32> m_Variant = { false };

		Array<int32> m_Array = { 0 };
		Map<str::String, int32> m_Map = { { "A", 0 } };
		Set<int32> m_Set = { 0 };
	};
}

template<>
void imgui::Inspector::ReadCustom(const Struct& value)
{
	Read("m_Bool", value.m_Bool);
	Read("m_Float", value.m_Float);
	Read("m_Double", value.m_Double);
	Read("m_Int8", value.m_Int8);
	Read("m_Int16", value.m_Int16);
	Read("m_Int32", value.m_Int32);
	Read("m_Int64", value.m_Int64);
	Read("m_UInt8", value.m_UInt8);
	Read("m_UInt16", value.m_UInt16);
	Read("m_UInt32", value.m_UInt32);
	Read("m_UInt64", value.m_UInt64);
	Read("m_Enum", value.m_Enum);
	Read("m_Guid", value.m_Guid);
	Read("m_Name", value.m_Name);
	Read("m_Path", value.m_Path);
	Read("m_String", value.m_String);
	Read("m_Quaternion", value.m_Quaternion);
	Read("m_Rotator", value.m_Rotator);
	Read("m_Vector2f", value.m_Vector2f);
	Read("m_Vector2i", value.m_Vector2i);
	Read("m_Vector2u", value.m_Vector2u);
	Read("m_Vector3f", value.m_Vector3f);
	Read("m_Vector3i", value.m_Vector3i);
	Read("m_Vector4f", value.m_Vector4f);
	Read("m_Optional", value.m_Optional);
	Read("m_StructSimple", value.m_StructSimple);
	Read("m_StructRecursive", value.m_StructRecursive);
	Read("m_Variant", value.m_Variant);
	Read("m_Array", value.m_Array);
	Read("m_Map", value.m_Map);
	Read("m_Set", value.m_Set);
}

template<>
bool imgui::Inspector::WriteCustom(Struct& value)
{
	bool result = false;
	result |= Write("m_Bool", value.m_Bool);
	result |= Write("m_Float", value.m_Float);
	result |= Write("m_Double", value.m_Double);
	result |= Write("m_Int8", value.m_Int8);
	result |= Write("m_Int16", value.m_Int16);
	result |= Write("m_Int32", value.m_Int32);
	result |= Write("m_Int64", value.m_Int64);
	result |= Write("m_UInt8", value.m_UInt8);
	result |= Write("m_UInt16", value.m_UInt16);
	result |= Write("m_UInt32", value.m_UInt32);
	result |= Write("m_UInt64", value.m_UInt64);
	result |= Write("m_Enum", value.m_Enum);
	result |= Write("m_Guid", value.m_Guid);
	result |= Write("m_Name", value.m_Name);
	result |= Write("m_Path", value.m_Path);
	result |= Write("m_String", value.m_String);
	result |= Write("m_Quaternion", value.m_Quaternion);
	result |= Write("m_Rotator", value.m_Rotator);
	result |= Write("m_Vector2f", value.m_Vector2f);
	result |= Write("m_Vector2i", value.m_Vector2i);
	result |= Write("m_Vector2u", value.m_Vector2u);
	result |= Write("m_Vector3f", value.m_Vector3f);
	result |= Write("m_Vector3i", value.m_Vector3i);
	result |= Write("m_Vector4f", value.m_Vector4f);
	result |= Write("m_Optional", value.m_Optional);
	result |= Write("m_StructSimple", value.m_StructSimple);
	result |= Write("m_StructRecursive", value.m_StructRecursive);
	result |= Write("m_Variant", value.m_Variant);
	result |= Write("m_Array", value.m_Array);
	result |= Write("m_Map", value.m_Map);
	result |= Write("m_Set", value.m_Set);
	return result;
}

void imgui::InspectorDemo_Struct(bool isReading)
{
	if (ImGui::BeginTabItem("Struct"))
	{
		static Struct m_Struct = {};

		imgui::Inspector inspector;
		if (inspector.Begin("##table"))
		{
			VISIT("m_Struct", m_Struct);
			inspector.End();
		}
		ImGui::EndTabItem();
	}
}

#undef VISIT