#pragma once

template<typename TRenderStage, typename... TArgs>
void eng::RenderSystem::RegisterStage(TArgs&&... args)
{
	m_RenderStages.Append(new TRenderStage(std::forward<TArgs>(args)...));
}