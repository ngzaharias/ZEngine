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

void imgui::InspectorDemo_Variant(bool isReading)
{
	if (ImGui::BeginTabItem("Variant"))
	{
		static Variant<bool, float> m_Float = 0.f;
		static Variant<bool, double> m_Double = 0.0;
		static Variant<bool, int8> m_Int8 = int8(0);
		static Variant<bool, int16> m_Int16 = int16(0);
		static Variant<bool, int32> m_Int32 = int32(0);
		static Variant<bool, int64> m_Int64 = int64(0);
		static Variant<bool, uint8> m_UInt8 = uint8(0);
		static Variant<bool, uint16> m_UInt16 = uint16(0);
		static Variant<bool, uint32> m_UInt32 = uint32(0);
		static Variant<bool, uint64> m_UInt64 = uint64(0);
		static Variant<bool, EEnum> m_Enum = EEnum::First;
		static Variant<bool, str::Guid> m_Guid = { str::Guid::Unassigned };
		static Variant<bool, str::Name> m_Name = { str::Name::Unassigned };
		static Variant<bool, str::Path> m_Path = { "" };
		static Variant<bool, str::String> m_String = { "" };
		static Variant<bool, Quaternion> m_Quaternion = Quaternion::Identity;
		static Variant<bool, Rotator> m_Rotator = Rotator::Zero;
		static Variant<bool, Vector2f> m_Vector2f = Vector2f::Zero;
		static Variant<bool, Vector2i> m_Vector2i = Vector2i::Zero;
		static Variant<bool, Vector2u> m_Vector2u = Vector2u::Zero;
		static Variant<bool, Vector3f> m_Vector3f = Vector3f::Zero;
		static Variant<bool, Vector3i> m_Vector3i = Vector3i::Zero;
		static Variant<bool, Vector4f> m_Vector4f = Vector4f::Zero;
		static Variant<bool, StructSimple> m_StructSimple = StructSimple{};
		static Variant<bool, StructRecursive> m_StructRecursive = StructRecursive{};
		static Variant<bool, Optional<int32>> m_Optional = 0;
		static Variant<bool, Variant<bool, int32>> m_Variant = 0;
		static Variant<bool, Array<int32>> m_Array = Array<int32>{ 0 };
		static Variant<bool, Map<str::String, int32>> m_Map = Map<str::String, int32>{ { "A", 0 } };
		static Variant<bool, Set<int32>> m_Set = Set<int32>{ 0 };

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
			VISIT("m_Variant", m_Variant);
			VISIT("m_Array", m_Array);
			VISIT("m_Map", m_Map);
			VISIT("m_Set", m_Set);
			inspector.End();
		}

		ImGui::EndTabItem();
	}
}

#undef VISIT