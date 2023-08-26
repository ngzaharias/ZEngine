#pragma once

#include <Engine/GameTime.h>

#include <Vulkan/vulkan.h>

namespace glfw
{
	class Window;
}

namespace render
{
	class IWindow;
}

namespace vulkan
{
	class Device;
	class Model;
	class Pipeline;
	class SwapChain;
	class Texture;

	class Renderer final
	{
	public:
		Renderer(vulkan::Device& device, glfw::Window& window);
		~Renderer();

		void Begin(const core::GameTime& gameTime);
		void Finish();

		void Render(const Matrix4x4& transform);

	private:
		VkCommandBuffer BeginFrame();
		void FinishFrame(VkCommandBuffer commandBuffer);

		void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void FinishSwapChainRenderPass(VkCommandBuffer commandBuffer);

		void CreateCommandBuffers();
		void DestroyCommandBuffers();

		void RecreatePipeline();
		void RecreateSwapChain();

	private:
		vulkan::Device& m_Device;
		glfw::Window& m_Window;

		vulkan::Model* m_Model = nullptr;
		vulkan::Pipeline* m_Pipeline = nullptr;
		vulkan::SwapChain* m_SwapChain = nullptr;
		vulkan::Texture* m_Texture = nullptr;

		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;

		VkCommandBuffer m_CommandBufferActive = VK_NULL_HANDLE;
		Array<VkCommandBuffer> m_CommandBuffers = { };

		uint32 m_Frame = 0;
		uint32 m_FrameIndex = 0;
		uint32 m_ImageIndex = 0;
		bool m_WasFrameBufferResized = false;
	};
}