#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorTemplates.h"

#include "Camera/CameraBound2DTemplate.h"
#include "Camera/CameraMove2DTemplate.h"
#include "Camera/CameraMove3DTemplate.h"
#include "Camera/CameraPan3DTemplate.h"
#include "Camera/CameraZoom2DTemplate.h"
#include "ECS/NameComponent.h"
#include "Engine/CameraTemplate.h"
#include "Engine/FlipbookTemplate.h"
#include "Engine/LightAmbientTemplate.h"
#include "Engine/LightDirectionalTemplate.h"
#include "Engine/LightPointTemplate.h"
#include "Engine/PhysicsTemplate.h"
#include "Engine/SpriteTemplate.h"
#include "Engine/StaticMeshTemplate.h"
#include "Engine/TransformTemplate.h"
#include "Engine/VisibilityTemplate.h"
#include "EntityEditor/EntityEditorInspector.h"

#include "imgui/Property.h"

#define VISIT(component, field) \
if (imgui::Write(#field, valueNew.field)) \
{ \
	PushCommand(&component::field, valueOld.field, valueNew.field); \
} \

template<>
void editor::entity::Inspector::VisitCustom(const camera::Bound2DTemplate& valueOld)
{
	camera::Bound2DTemplate valueNew = valueOld;
	VISIT(camera::Bound2DTemplate, m_Min);
	VISIT(camera::Bound2DTemplate, m_Max);
}

template<>
void editor::entity::Inspector::VisitCustom(const camera::Move2DTemplate& valueOld)
{
}

template<>
void editor::entity::Inspector::VisitCustom(const camera::Move3DTemplate& valueOld)
{
}

template<>
void editor::entity::Inspector::VisitCustom(const camera::Pan3DTemplate& valueOld)
{
}

template<>
void editor::entity::Inspector::VisitCustom(const camera::Zoom2DTemplate& valueOld)
{
	camera::Zoom2DTemplate valueNew = valueOld;
	VISIT(camera::Zoom2DTemplate, m_Min);
	VISIT(camera::Zoom2DTemplate, m_Max);
}

template<>
void editor::entity::Inspector::VisitCustom(const client::hidden::CountTemplate& valueOld)
{
}

template<>
void editor::entity::Inspector::VisitCustom(const client::hidden::GroupTemplate& valueOld)
{
	client::hidden::GroupTemplate valueNew = valueOld;
	VISIT(client::hidden::GroupTemplate, m_Objects);
}

template<>
void editor::entity::Inspector::VisitCustom(const client::hidden::ObjectTemplate& valueOld)
{
}

template<>
void editor::entity::Inspector::VisitCustom(const ecs::NameComponent& valueOld)
{
	ecs::NameComponent valueNew = valueOld;
	VISIT(ecs::NameComponent, m_Name);
}

template<>
void editor::entity::Inspector::VisitCustom(const eng::CameraTemplate& valueOld)
{
	eng::CameraTemplate valueNew = valueOld;
	VISIT(eng::CameraTemplate, m_Projection);
}

template<>
void editor::entity::Inspector::VisitCustom(const eng::FlipbookTemplate& valueOld)
{
	eng::FlipbookTemplate valueNew = valueOld;
	VISIT(eng::FlipbookTemplate, m_Flipbook);
	VISIT(eng::FlipbookTemplate, m_Size);
	VISIT(eng::FlipbookTemplate, m_Index);
	VISIT(eng::FlipbookTemplate, m_TimeStart);
	VISIT(eng::FlipbookTemplate, m_IsLooping);
	VISIT(eng::FlipbookTemplate, m_IsPlaying);
}

template<>
void editor::entity::Inspector::VisitCustom(const eng::light::AmbientTemplate& valueOld)
{
	eng::light::AmbientTemplate valueNew = valueOld;
	VISIT(eng::light::AmbientTemplate, m_Colour);
}

template<>
void editor::entity::Inspector::VisitCustom(const eng::light::DirectionalTemplate& valueOld)
{
	eng::light::DirectionalTemplate valueNew = valueOld;
	VISIT(eng::light::DirectionalTemplate, m_Colour);
}

template<>
void editor::entity::Inspector::VisitCustom(const eng::light::PointTemplate& valueOld)
{
	eng::light::PointTemplate valueNew = valueOld;
	VISIT(eng::light::PointTemplate, m_Colour);
	VISIT(eng::light::PointTemplate, m_Range);
}

template<>
void editor::entity::Inspector::VisitCustom(const eng::PhysicsTemplate& valueOld)
{
	eng::PhysicsTemplate valueNew = valueOld;
	VISIT(eng::PhysicsTemplate, m_Rigidbody);
	VISIT(eng::PhysicsTemplate, m_Shapes);
}

template<>
void editor::entity::Inspector::VisitCustom(const eng::SpriteTemplate& valueOld)
{
	eng::SpriteTemplate valueNew = valueOld;
	VISIT(eng::SpriteTemplate, m_Sprite);
	VISIT(eng::SpriteTemplate, m_Colour);
	VISIT(eng::SpriteTemplate, m_Size);
}

template<>
void editor::entity::Inspector::VisitCustom(const eng::StaticMeshTemplate& valueOld)
{
	eng::StaticMeshTemplate valueNew = valueOld;
	VISIT(eng::StaticMeshTemplate, m_StaticMesh);
}

template<>
void editor::entity::Inspector::VisitCustom(const eng::TransformTemplate& valueOld)
{
	eng::TransformTemplate valueNew = valueOld;
	VISIT(eng::TransformTemplate, m_Translate);
	VISIT(eng::TransformTemplate, m_Rotate);
	VISIT(eng::TransformTemplate, m_Scale);
}

template<>
void editor::entity::Inspector::VisitCustom(const eng::UUIDComponent& valueOld)
{
	eng::UUIDComponent valueNew = valueOld;
	VISIT(eng::UUIDComponent, m_UUID);
}

template<>
void editor::entity::Inspector::VisitCustom(const eng::VisibilityTemplate& valueOld)
{
	eng::VisibilityTemplate valueNew = valueOld;
	if (imgui::Checkbox("##m_IsVisible", valueNew.m_IsVisible))
	{
		PushCommand(&eng::VisibilityTemplate::m_IsVisible, valueOld.m_IsVisible, valueNew.m_IsVisible);
	}
}

template<>
void editor::entity::Inspector::VisitCustom(const hexmap::RootTemplate& valueOld)
{
	hexmap::RootTemplate valueNew = valueOld;
	VISIT(hexmap::RootTemplate, m_HexRadius);
	VISIT(hexmap::RootTemplate, m_HexCount);
	VISIT(hexmap::RootTemplate, m_Zoom);
	VISIT(hexmap::RootTemplate, m_Zone);
}

#undef VISIT