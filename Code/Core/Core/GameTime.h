#pragma once

using uint32 = uint32_t;

class GameTime final
{
public:
	float m_DeltaTime = 0.f;
	float m_TotalTime = 0.f;
	uint32 m_Frame = 0;
};
