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

void imgui::InspectorDemo_Array(bool isReading)
{
	if (ImGui::BeginTabItem("Array"))
	{
		static Array<float> m_Float = { 0.f };
		static Array<double> m_Double = { 0.0 };
		static Array<int8> m_Int8 = { 0 };
		static Array<int16> m_Int16 = { 0 };
		static Array<int32> m_Int32 = { 0 };
		static Array<int64> m_Int64 = { 0 };
		static Array<uint8> m_UInt8 = { 0 };
		static Array<uint16> m_UInt16 = { 0 };
		static Array<uint32> m_UInt32 = { 0 };
		static Array<uint64> m_UInt64 = { 0 };
		static Array<EEnum> m_Enum = { EEnum::First };
		static Array<str::Guid> m_Guid = { {} };
		static Array<str::Name> m_Name = { {} };
		static Array<str::Path> m_Path = { {} };
		static Array<str::String> m_String = { {} };
		static Array<Quaternion> m_Quaternion = { {} };
		static Array<Rotator> m_Rotator = { {} };
		static Array<Vector2f> m_Vector2f = { {} };
		static Array<Vector2i> m_Vector2i = { {} };
		static Array<Vector2u> m_Vector2u = { {} };
		static Array<Vector3f> m_Vector3f = { {} };
		static Array<Vector3i> m_Vector3i = { {} };
		static Array<Vector4f> m_Vector4f = { {} };
		static Array<StructSimple> m_StructSimple = { {}, {} };
		static Array<StructRecursive> m_StructRecursive = { {} };
		static Array<Optional<int32>> m_Optional = { 0 };
		static Array<Variant<bool, int32>> m_Variant = { false };
		static Array<Array<int32>> m_Array = { { 0 } };
		static Array<Map<str::String, int32>> m_Map = { { { "A", 0 } } };
		static Array<Set<int32>> m_Set = { { 0 } };

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