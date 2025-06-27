#include "EnginePCH.h"
#include "Engine/Vulkan/Pipeline.h"

#include "Engine/FileHelpers.h"
#include "Engine/Vulkan/Device.h"
#include "Engine/Vulkan/Model.h"

namespace 
{
	// #todo: auto generated based on vulkan::Vertex struct
	Array<VkVertexInputAttributeDescription> GetAttributeDescriptions()
	{
		Array<VkVertexInputAttributeDescription> attributeDescriptions;

		VkVertexInputAttributeDescription description0;
		description0.binding = 0;
		description0.location = 0; // #note: location in the shader
		description0.format = VK_FORMAT_R32G32B32_SFLOAT;
		description0.offset = offsetof(vulkan::Vertex, m_Position);
		attributeDescriptions.push_back(description0);

		VkVertexInputAttributeDescription description1;
		description1.binding = 0;
		description1.location = 1; // #note: location in the shader
		description1.format = VK_FORMAT_R32G32B32_SFLOAT;
		description1.offset = offsetof(vulkan::Vertex, m_Colour);
		attributeDescriptions.push_back(description1);

		return attributeDescriptions;
	}

	Array<VkVertexInputBindingDescription> GetBindingDescriptions()
	{
		Array<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(vulkan::Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}

	VkShaderModule LoadShaderModule(const VkDevice logicalDevice, const Array<char>& shaderData)
	{
		VkShaderModuleCreateInfo createInfo;
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = shaderData.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderData.data());
		createInfo.flags = 0;
		createInfo.pNext = nullptr;

		VkShaderModule shaderModule = VK_NULL_HANDLE;
		vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule);
		return shaderModule;
	};
}

vulkan::Pipeline::Pipeline(Device& device, const vulkan::PipelineConfig& config)
	: m_Device(device)
{
	// shaders
	{
		Array<char> vertShader;
		if (!core::LoadFileAsChar("Assets/Shaders/SimpleShader.vert.spv", vertShader))
			throw std::runtime_error("[Vulkan] Failed to load vertex shader!");

		m_VertShaderModule = LoadShaderModule(m_Device.m_LogicalDevice, vertShader);
		if (m_VertShaderModule == VK_NULL_HANDLE)
			throw std::runtime_error("[Vulkan] Failed to create vertex shader module!");

		Array<char> fragShader;
		if (!core::LoadFileAsChar("Assets/Shaders/SimpleShader.frag.spv", fragShader))
			throw std::runtime_error("[Vulkan] Failed to load fragment shader!");

		m_FragShaderModule = LoadShaderModule(m_Device.m_LogicalDevice, fragShader);
		if (m_FragShaderModule == VK_NULL_HANDLE)
			throw std::runtime_error("[Vulkan] Failed to create fragment shader module!");
	}

	// graphics pipeline
	{
		// Shaders
		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = m_VertShaderModule;
		vertShaderStageInfo.pName = "main"; // #note: shader entry point
		vertShaderStageInfo.pSpecializationInfo = nullptr;

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = m_FragShaderModule;
		fragShaderStageInfo.pName = "main"; // #note: shader entry point
		fragShaderStageInfo.pSpecializationInfo = nullptr;

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		auto attributeDescriptions = GetAttributeDescriptions();
		auto bindingDescriptions = GetBindingDescriptions();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo;
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32>(attributeDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
		vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32>(bindingDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
		vertexInputInfo.flags = 0;
		vertexInputInfo.pNext = nullptr;

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;

		pipelineInfo.pColorBlendState = &config.m_ColorBlendInfo;
		pipelineInfo.pDepthStencilState = &config.m_DepthStencilInfo;
		pipelineInfo.pDynamicState = &config.m_DynamicStateInfo;
		pipelineInfo.pInputAssemblyState = &config.m_InputAssemblyInfo;
		pipelineInfo.pMultisampleState = &config.m_MultisampleInfo;
		pipelineInfo.pRasterizationState = &config.m_RasterizationInfo;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pViewportState = &config.m_ViewportInfo;

		pipelineInfo.layout = config.m_PipelineLayout;
		pipelineInfo.renderPass = config.m_RenderPass;
		pipelineInfo.subpass = config.m_Subpass;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

		vkCreateGraphicsPipelines(m_Device.m_LogicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline);

		if (m_GraphicsPipeline == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to create graphics pipeline!");
	}
}

vulkan::Pipeline::~Pipeline()
{
	vkDestroyShaderModule(m_Device.m_LogicalDevice, m_FragShaderModule, nullptr);
	vkDestroyShaderModule(m_Device.m_LogicalDevice, m_VertShaderModule, nullptr);
	vkDestroyPipeline(m_Device.m_LogicalDevice, m_GraphicsPipeline, nullptr);
}

void vulkan::Pipeline::DefaultPipelineConfig(PipelineConfig& out_Config)
{
	VkPipelineViewportStateCreateInfo viewport{};
	viewport.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport.viewportCount = 1;
	viewport.pViewports = nullptr; // #todo
	viewport.scissorCount = 1;
	viewport.pScissors = nullptr;  // #todo
	out_Config.m_ViewportInfo = viewport;

	// Input Assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	out_Config.m_InputAssemblyInfo = inputAssembly;

	// Rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.f;
	rasterizer.cullMode = VK_CULL_MODE_NONE;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.f;
	rasterizer.depthBiasClamp = 0.f;
	rasterizer.depthBiasSlopeFactor = 0.f;
	out_Config.m_RasterizationInfo = rasterizer;

	// Multi-sampling
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.f;
	multisampling.pSampleMask = nullptr;
	multisampling.alphaToCoverageEnable = VK_FALSE;
	multisampling.alphaToOneEnable = VK_FALSE;
	out_Config.m_MultisampleInfo = multisampling;

	// Depth and Stencil
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
	depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilInfo.depthTestEnable = VK_TRUE;
	depthStencilInfo.depthWriteEnable = VK_TRUE;
	depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	depthStencilInfo.minDepthBounds = 0.0f;  // Optional
	depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
	depthStencilInfo.stencilTestEnable = VK_FALSE;
	depthStencilInfo.front = {};  // Optional
	depthStencilInfo.back = {};   // Optional
	out_Config.m_DepthStencilInfo = depthStencilInfo;

	// Colour Blending
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	out_Config.m_ColorBlendAttachment = colorBlendAttachment;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &out_Config.m_ColorBlendAttachment;
	colorBlending.blendConstants[0] = 0.f;
	colorBlending.blendConstants[1] = 0.f;
	colorBlending.blendConstants[2] = 0.f;
	colorBlending.blendConstants[3] = 0.f;
	out_Config.m_ColorBlendInfo = colorBlending;

	// Dynamic State

	// #note: set BEFORE setting dynamicStateCount
	out_Config.m_DynamicStates = {
		//VK_DYNAMIC_STATE_LINE_WIDTH,
		VK_DYNAMIC_STATE_SCISSOR,
		VK_DYNAMIC_STATE_VIEWPORT };

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32>(out_Config.m_DynamicStates.size());
	dynamicState.pDynamicStates = out_Config.m_DynamicStates.data();
	out_Config.m_DynamicStateInfo = dynamicState;

	// Sub-pass
	out_Config.m_Subpass = 0;
}