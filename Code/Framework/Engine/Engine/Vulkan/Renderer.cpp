#include "EnginePCH.h"
#include "Engine/Vulkan/Renderer.h"

#include "Engine/GLFW/Window.h"
#include "Engine/Vulkan/Device.h"
#include "Engine/Vulkan/Geometry.h"
#include "Engine/Vulkan/Model.h"
#include "Engine/Vulkan/Pipeline.h"
#include "Engine/Vulkan/SwapChain.h"
#include "Engine/Vulkan/Texture.h"

#include <GLFW/glfw3.h>
#include <Vulkan/vulkan.h>

namespace
{
	constexpr Vector3f s_Scale = Vector3f(1.f);

	const Array<Matrix4x4> s_Matrixes = {
		Matrix4x4({-2.f, +2.f, 10.f}, Quaternion::Identity, 1.f),
		Matrix4x4({+2.f, +2.f, 10.f}, Quaternion::Identity, 1.f),
		Matrix4x4({+0.f, -2.f, 10.f}, Quaternion::Identity, 1.f) };

	constexpr Vector3f s_Colours[] = {
		Vector3f(1.f, 0.f, 0.f),
		Vector3f(0.f, 1.f, 0.f),
		Vector3f(0.f, 0.f, 1.f),
	};

	struct PushConstants
	{
		ALIGN_16 Matrix4x4 m_Transform = Matrix4x4::Identity;
		ALIGN_16 Vector3f m_Colour = Vector3f::Zero;
		float m_DeltaTime = 0.f;
	};
}

vulkan::Renderer::Renderer(vulkan::Device& device, glfw::Window& window)
	: m_Device(device)
	, m_Window(window)
{
	m_Model = new vulkan::Model(m_Device, vulkan::s_CubeIndexedV, vulkan::s_CubeIndexedI);
	m_Texture = new vulkan::Texture(m_Device);

	RecreateSwapChain();
	CreateCommandBuffers();
}

vulkan::Renderer::~Renderer()
{
	vkDestroyPipelineLayout(m_Device.m_LogicalDevice, m_PipelineLayout, nullptr);

	DestroyCommandBuffers();

	delete m_Pipeline;
	delete m_SwapChain;
	delete m_Texture;
	delete m_Model;
}

void vulkan::Renderer::Begin(const core::GameTime& gameTime)
{
	m_Frame = gameTime.m_Frame;
	m_FrameIndex = m_Frame % SwapChain::s_MaxFramesInFlight;

	if (m_CommandBufferActive != VK_NULL_HANDLE)
		throw std::runtime_error("[Vulkan] Cannot begin rendering again when the last one hasn't finished");

	m_CommandBufferActive = BeginFrame();
	BeginSwapChainRenderPass(m_CommandBufferActive);

	vkCmdBindPipeline(m_CommandBufferActive, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline->m_GraphicsPipeline);

	m_Model->Bind(m_CommandBufferActive);
}

void vulkan::Renderer::Finish()
{
	FinishSwapChainRenderPass(m_CommandBufferActive);
	FinishFrame(m_CommandBufferActive);

	m_CommandBufferActive = nullptr;
}

void vulkan::Renderer::Render(const Matrix4x4& transform)
{
	if (m_CommandBufferActive == VK_NULL_HANDLE)
		throw std::runtime_error("[Vulkan] Cannot render an object when rendering hasn't started.");

	PushConstants pushConstants;
	pushConstants.m_Transform = transform;

	vkCmdPushConstants(
		m_CommandBufferActive,
		m_PipelineLayout,
		VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
		0,
		sizeof(PushConstants),
		&pushConstants);

	m_Model->Draw(m_CommandBufferActive);
}

VkCommandBuffer vulkan::Renderer::BeginFrame()
{
	{
		const VkResult result = m_SwapChain->AcquireNextImage(m_Frame, m_ImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return VK_NULL_HANDLE;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw std::runtime_error("failed to acquire swap chain image!");
	}

	VkCommandBuffer commandBuffer = m_CommandBuffers[m_FrameIndex];
	if (commandBuffer == VK_NULL_HANDLE)
		throw std::runtime_error("Failed to create command buffer!");

	VkCommandBufferBeginInfo beginInfo;
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;
	beginInfo.pNext = nullptr;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		throw std::runtime_error("Failed to begin command buffer!");

	return commandBuffer;
}

void vulkan::Renderer::FinishFrame(VkCommandBuffer commandBuffer)
{
	const uint32 frameIndex = vulkan::SwapChain::FrameIndex(m_Frame);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		throw std::runtime_error("Failed to end command buffer!");

	// Check if a previous frame is using this image (i.e. there is its fence to wait on)
	if (m_SwapChain->m_InFlightImages[m_ImageIndex] != VK_NULL_HANDLE)
		vkWaitForFences(m_Device.m_LogicalDevice, 1, &m_SwapChain->m_InFlightImages[m_ImageIndex], VK_TRUE, UINT64_MAX);

	// Mark the image as now being in use by this frame
	m_SwapChain->m_InFlightImages[m_ImageIndex] = m_SwapChain->m_InFlightFences[frameIndex];

	VkSemaphore waitSemaphores[] = { m_SwapChain->m_ImageAvailableSemaphores[frameIndex] };
	VkSemaphore signalSemaphores[] = { m_SwapChain->m_RenderFinishedSemaphores[frameIndex] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSwapchainKHR swapChains[] = { m_SwapChain->m_SwapChain };

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_CommandBuffers[m_FrameIndex];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(m_Device.m_LogicalDevice, 1, &m_SwapChain->m_InFlightFences[frameIndex]);

	if (vkQueueSubmit(m_Device.m_GraphicsQueue, 1, &submitInfo, m_SwapChain->m_InFlightFences[frameIndex]) != VK_SUCCESS)
		throw std::runtime_error("Failed to submit draw command buffer!");

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &m_ImageIndex;
	presentInfo.pResults = nullptr;

	{
		const VkResult result = vkQueuePresentKHR(m_Device.m_PresentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_Window.HasResized())
		{
			m_Window.SetResized(false);
			RecreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS)
			throw std::runtime_error("Failed to present swap chain image!");
	}
}

void vulkan::Renderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
	// clear values
	Array<VkClearValue> clearValues(2);
	{
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.f, 0 };
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_SwapChain->m_RenderPass;
	renderPassInfo.framebuffer = m_SwapChain->m_SwapChainFramebuffers[m_ImageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = m_SwapChain->m_SwapChainExtent;
	renderPassInfo.clearValueCount = static_cast<uint32>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	// Viewport
	VkViewport viewport{};
	viewport.x = 0.f;
	viewport.y = 0.f;
	viewport.width = (float)m_SwapChain->m_SwapChainExtent.width;
	viewport.height = (float)m_SwapChain->m_SwapChainExtent.height;
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	// Scissor
	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = m_SwapChain->m_SwapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void vulkan::Renderer::FinishSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
	vkCmdEndRenderPass(commandBuffer);
}

void vulkan::Renderer::CreateCommandBuffers()
{
	if (!m_SwapChain)
		throw std::runtime_error("Failed to create command buffer!");

	m_CommandBuffers = Array<VkCommandBuffer>(SwapChain::s_MaxFramesInFlight, VK_NULL_HANDLE);

	VkCommandBufferAllocateInfo allocInfo;
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_Device.m_CommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32)m_CommandBuffers.size();
	allocInfo.pNext = nullptr;

	if (vkAllocateCommandBuffers(m_Device.m_LogicalDevice, &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
		throw std::runtime_error("Failed to create command buffer!");
}

void vulkan::Renderer::DestroyCommandBuffers()
{
	if (m_CommandBuffers.size() != 0)
	{
		vkFreeCommandBuffers(
			m_Device.m_LogicalDevice,
			m_Device.m_CommandPool,
			static_cast<uint32>(m_CommandBuffers.size()),
			m_CommandBuffers.data());
		m_CommandBuffers.clear();
	}
}

void vulkan::Renderer::RecreatePipeline()
{
	if (!m_SwapChain)
		throw std::runtime_error("Failed to create pipeline layout!");

	// pipeline layout
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PushConstants);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (m_PipelineLayout != VK_NULL_HANDLE)
			vkDestroyPipelineLayout(m_Device.m_LogicalDevice, m_PipelineLayout, nullptr);
		vkCreatePipelineLayout(m_Device.m_LogicalDevice, &pipelineLayoutInfo, nullptr, &m_PipelineLayout);

		if (m_PipelineLayout == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to create pipeline layout!");
	}

	// pipeline
	{
		vulkan::PipelineConfig pipelineConfig;
		vulkan::Pipeline::DefaultPipelineConfig(pipelineConfig);
		pipelineConfig.m_PipelineLayout = m_PipelineLayout;
		pipelineConfig.m_RenderPass = m_SwapChain->m_RenderPass;

		// #todo: safe delete
		delete m_Pipeline;
		m_Pipeline = new vulkan::Pipeline(m_Device, pipelineConfig);
	}
}

void vulkan::Renderer::RecreateSwapChain()
{
	Vector2u windowSize = m_Window.GetSize();
	while (windowSize.x == 0 || windowSize.y == 0)
	{
		windowSize = m_Window.GetSize();
		glfwWaitEvents();
	}
	vkDeviceWaitIdle(m_Device.m_LogicalDevice);

	vulkan::SwapChain* swapChainOld = m_SwapChain;
	m_SwapChain = new vulkan::SwapChain(m_Device, m_Window, swapChainOld);

	if (swapChainOld && !swapChainOld->HasMatchingFormats(*m_SwapChain))
		throw std::runtime_error("[Vulkan] Swap-chain image/depth format has changed!");

	// #todo: safe delete
	delete swapChainOld;

	// #todo: don't re-create if render pass is compatible 
	RecreatePipeline();
}