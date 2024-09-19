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

#define VISIT(label, value) \
{ \
	if (isReading) { inspector.Read(label, value); } \
	else { inspector.Write(label, value); } \
}

void imgui::InspectorDemo_Map(bool isReading)
{
	if (ImGui::BeginTabItem("Map"))
	{
		static Map<str::String, bool> m_Bool = { { "A", 0.f } };
		static Map<str::String, float> m_Float = { { "A", 0.f } };
		static Map<str::String, double> m_Double = { { "A", 0.0 } };
		static Map<str::String, int8> m_Int8 = { { "A", 0 } };
		static Map<str::String, int16> m_Int16 = { { "A", 0 } };
		static Map<str::String, int32> m_Int32 = { { "A", 0 } };
		static Map<str::String, int64> m_Int64 = { { "A", 0 } };
		static Map<str::String, uint8> m_UInt8 = { { "A", 0 } };
		static Map<str::String, uint16> m_UInt16 = { { "A", 0 } };
		static Map<str::String, uint32> m_UInt32 = { { "A", 0 } };
		static Map<str::String, uint64> m_UInt64 = { { "A", 0 } };
		static Map<str::String, EEnum> m_Enum = { { "A", EEnum::First } };
		static Map<str::String, str::Guid> m_Guid = { { "A", {} } };
		static Map<str::String, str::Name> m_Name = { { "A", {} } };
		static Map<str::String, str::Path> m_Path = { { "A", {} } };
		static Map<str::String, str::String> m_String = { { "A", {} } };
		static Map<str::String, Quaternion> m_Quaternion = { { "A", {} } };
		static Map<str::String, Rotator> m_Rotator = { { "A", {} } };
		static Map<str::String, Vector2f> m_Vector2f = { { "A", {} } };
		static Map<str::String, Vector2i> m_Vector2i = { { "A", {} } };
		static Map<str::String, Vector2u> m_Vector2u = { { "A", {} } };
		static Map<str::String, Vector3f> m_Vector3f = { { "A", {} } };
		static Map<str::String, Vector3i> m_Vector3i = { { "A", {} } };
		static Map<str::String, Vector4f> m_Vector4f = { { "A", {} } };
		static Map<str::String, StructSimple> m_StructSimple = { { "A", {} } };
		static Map<str::String, StructRecursive> m_StructRecursive = { { "A", {} } };
		static Map<str::String, Optional<int32>> m_Optional = { { "A", 0 } };
		static Map<str::String, Variant<bool, int32>> m_Variant = { { "A", false } };
		static Map<str::String, Array<int32>> m_Array = { { "A", { 0 } } };
		static Map<str::String, Map<str::String, int32>> m_Map = { { "A", { { "A", 0 } } } };
		static Map<str::String, Set<int32>> m_Set = { { "A", { 0 } } };

		imgui::Inspector inspector;
		if (inspector.Begin("##table"))
		{
			VISIT("m_Float", m_Float);
			VISIT("m_Double", m_Double);
			VISIT("m_Int8", m_Int8);
			VISIT("m_Int16", m_Int16);
			VISIT("m_Int32", m_Int32);
			VISIT("m_Int64", m_Int64);
			VISIT("m_UInt8", m_UInt8);
			VISIT("m_UInt16", m_UInt16);
			VISIT("m_UInt32", m_UInt32);
			VISIT("m_UInt64", m_UInt64);
			VISIT("m_Enum", m_Enum);
			VISIT("m_Guid", m_Guid);
			VISIT("m_Name", m_Name);
			VISIT("m_Path", m_Path);
			VISIT("m_String", m_String);
			VISIT("m_Quaternion", m_Quaternion);
			VISIT("m_Rotator", m_Rotator);
			VISIT("m_Vector2f", m_Vector2f);
			VISIT("m_Vector2i", m_Vector2i);
			VISIT("m_Vector2u", m_Vector2u);
			VISIT("m_Vector3f", m_Vector3f);
			VISIT("m_Vector3i", m_Vector3i);
			VISIT("m_Vector4f", m_Vector4f);
			VISIT("m_StructSimple", m_StructSimple);
			VISIT("m_StructRecursive", m_StructRecursive);
			VISIT("m_Optional", m_Optional);
			//VISIT("m_Variant", m_Variant);
			VISIT("m_Array", m_Array);
			VISIT("m_Map", m_Map);
			VISIT("m_Set", m_Set);
			inspector.End();
		}
		ImGui::EndTabItem();
	}
}

#undef VISIT