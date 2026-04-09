#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorTemplates.h"

#include "Engine/TransformTemplate.h"
#include "EntityEditor/EntityEditorInspector.h"

#include "imgui/Property.h"

template<>
void editor::entity::Inspector::VisitCustom(const eng::TransformTemplate& valueOld)
{
	eng::TransformTemplate valueNew = valueOld;
	if (imgui::Write("m_Translate", valueNew.m_Translate))
		PushCommand(&eng::TransformTemplate::m_Translate, valueOld.m_Translate, valueNew.m_Translate);
	if (imgui::Write("m_Rotate", valueNew.m_Rotate))
		PushCommand(&eng::TransformTemplate::m_Rotate, valueOld.m_Rotate, valueNew.m_Rotate);
	if (imgui::Write("m_Scale", valueNew.m_Scale))
		PushCommand(&eng::TransformTemplate::m_Scale, valueOld.m_Scale, valueNew.m_Scale);
}