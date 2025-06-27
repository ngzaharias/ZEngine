#pragma once

#ifdef Z_VULKAN

#include <Vulkan/vulkan.h>

namespace vulkan
{
	class Device;
	class Renderer;

	struct PipelineConfig
	{
		PipelineConfig() = default;

		VkPipelineColorBlendAttachmentState m_ColorBlendAttachment = {};
		VkPipelineColorBlendStateCreateInfo m_ColorBlendInfo = {};
		VkPipelineDepthStencilStateCreateInfo m_DepthStencilInfo = {};
		VkPipelineDynamicStateCreateInfo m_DynamicStateInfo = {};
		VkPipelineInputAssemblyStateCreateInfo m_InputAssemblyInfo = {};
		VkPipelineMultisampleStateCreateInfo m_MultisampleInfo = {};
		VkPipelineRasterizationStateCreateInfo m_RasterizationInfo = {};
		VkPipelineViewportStateCreateInfo m_ViewportInfo = {};

		Array<VkDynamicState> m_DynamicStates = {};

		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
		VkRenderPass m_RenderPass = VK_NULL_HANDLE;

		uint32 m_Subpass = 0;

	private:
		PipelineConfig(const PipelineConfig&) = delete;
		PipelineConfig& operator=(const PipelineConfig&) = delete;
	};

	class Pipeline final
	{
		friend class Renderer;

	public:
		Pipeline(vulkan::Device& device, const vulkan::PipelineConfig& config);
		~Pipeline();

		static void DefaultPipelineConfig(PipelineConfig& out_Config);

	private:
		Pipeline(const Pipeline&) = delete;
		Pipeline& operator=(const Pipeline&) = delete;

	private:
		vulkan::Device& m_Device;

		VkPipeline m_GraphicsPipeline = VK_NULL_HANDLE;
		VkShaderModule m_VertShaderModule = VK_NULL_HANDLE;
		VkShaderModule m_FragShaderModule = VK_NULL_HANDLE;
	};
}

#endif