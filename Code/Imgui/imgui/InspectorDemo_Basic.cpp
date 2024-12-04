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

void imgui::InspectorDemo_Basic(bool isReading)
{
	if (ImGui::BeginTabItem("Basic"))
	{
		static bool m_Bool = false;
		static float m_Float = 0.f;
		static double m_Double = 0.0;
		static int8 m_Int8 = 0;
		static int16 m_Int16 = 0;
		static int32 m_Int32 = 0;
		static int64 m_Int64 = 0;
		static uint8 m_UInt8 = 0;
		static uint16 m_UInt16 = 0;
		static uint32 m_UInt32 = 0;
		static uint64 m_UInt64 = 0;
		static EEnum m_Enum = EEnum::First;
		static str::Guid m_Guid = {};
		static str::Name m_Name = {};
		static str::Path m_Path = {};
		static str::String m_String = {};
		static Quaternion m_Quaternion = {};
		static Rotator m_Rotator = {};
		static Vector2f m_Vector2f = {};
		static Vector2i m_Vector2i = {};
		static Vector2u m_Vector2u = {};
		static Vector3f m_Vector3f = {};
		static Vector3i m_Vector3i = {};
		static Vector4f m_Vector4f = {};
		static StructSimple m_StructSimple = {};
		static StructRecursive m_StructRecursive = {};
		static Optional<int32> m_Optional = { 0 };
		static Variant<bool, int32> m_Variant = int32(0);
		static Array<int32> m_Array = { 0, 1 };
		static Map<str::String, int32> m_Map = { { "A", 0 } };
		static Set<int32> m_Set = { 0 };

		imgui::Inspector inspector;
		if (inspector.Begin("##table"))
		{
			VISIT("m_Bool", m_Bool);
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