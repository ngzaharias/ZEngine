#pragma once

namespace dbg
{
	class FPSCounter
	{
	public:
		void AddFrame(const float deltaTime)
		{
			const float fps = 1.f / deltaTime;
			m_ValueRaw = static_cast<int32>(fps);

			// the averaged FPS over the last X seconds
			m_DeltaTotal += fps;
			m_DeltaCount++;
			m_Timer += deltaTime;
			if (m_Timer > 0.3f)
			{
				m_ValueAverage = static_cast<int32>(m_DeltaTotal / m_DeltaCount);
				m_DeltaTotal = 0.f;
				m_DeltaCount = 0;
				m_Timer = 0.f;
			}
		}

		int32 GetAverage() const { return m_ValueAverage; }
		int32 GetRaw() const { return m_ValueRaw; }

	private:
		int32 m_ValueAverage = 0;
		int32 m_ValueRaw = 0;

		int32 m_DeltaCount = 0;
		float m_DeltaTotal = 0.f;
		float m_Timer = 0.f;
	};
}