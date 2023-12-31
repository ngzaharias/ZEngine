#pragma once

namespace core
{
	struct TransformComponent
	{
		Vector3f m_Translate = { 0.f, 0.f, 0.f };
		Vector3f m_Rotate = { 0.f, 0.f, 0.f };
		Vector3f m_Scale = { 1.f, 1.f, 1.f };
	};
}

REFL_AUTO
(
	type(core::TransformComponent)
	, field(m_Translate)
	, field(m_Rotate)
	, field(m_Scale)
)