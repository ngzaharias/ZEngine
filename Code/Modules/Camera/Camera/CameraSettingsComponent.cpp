#include "CameraPCH.h"
#include "Camera/CameraSettingsComponent.h"

#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strRotateSpeed = "m_RotateSpeed";
	const str::StringView strTranslateSpeed = "m_TranslateSpeed";
	const str::StringView strZoomAmount = "m_ZoomAmount";
	const str::StringView strZoomSpeed = "m_ZoomSpeed";
}

template<>
void Visitor::ReadCustom(camera::SettingsComponent& value) const
{
	Read(strRotateSpeed, value.m_RotateSpeed, value.m_RotateSpeed);
	Read(strTranslateSpeed, value.m_TranslateSpeed, value.m_TranslateSpeed);
	Read(strZoomAmount, value.m_ZoomAmount, value.m_ZoomAmount);
	Read(strZoomSpeed, value.m_ZoomSpeed, value.m_ZoomSpeed);
}
template<>
void Visitor::WriteCustom(const camera::SettingsComponent& value)
{
	Write(strRotateSpeed, value.m_RotateSpeed);
	Write(strTranslateSpeed, value.m_TranslateSpeed);
	Write(strZoomAmount, value.m_ZoomAmount);
	Write(strZoomSpeed, value.m_ZoomSpeed);
}