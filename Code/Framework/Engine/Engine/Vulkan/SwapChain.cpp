#include "EnginePCH.h"
#include "Engine/Vulkan/SwapChain.h"

#include "Engine/GLFW/Window.h"
#include "Engine/Vulkan/Device.h"

namespace
{
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const Array<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return availableFormat;
		}
		return availableFormats[0];
	}

	VkPresentModeKHR ChooseSwapPresentMode(const Array<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				return availablePresentMode;
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D ChooseSwapExtent(const VkExtent2D& windowExtent, const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != UINT32_MAX)
			return capabilities.currentExtent;

		VkExtent2D actualExtent = windowExtent;
		actualExtent.width =
			std::max(capabilities.minImageExtent.width,
				std::min(capabilities.maxImageExtent.width,
					actualExtent.width));
		actualExtent.height =
			std::max(capabilities.minImageExtent.height,
				std::min(capabilities.maxImageExtent.height,
					actualExtent.height));
		return actualExtent;
	}

	uint32 FindMemoryType(VkPhysicalDevice physicalDevice, uint32 filter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties{};
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if ((filter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
				return i;
		}

		throw std::runtime_error("[Vulkan] Failed to find suitable memory type!");
	}

	void CreateImageWithInfo(
		VkDevice logicalDevice, 
		VkPhysicalDevice physicalDevice,
		const VkImageCreateInfo& imageInfo, 
		VkImage& image, 
		VkDeviceMemory& imageMemory,
		VkMemoryPropertyFlags properties)
	{
		if (vkCreateImage(logicalDevice, &imageInfo, nullptr, &image) != VK_SUCCESS)
			throw std::runtime_error("[Vulkan] Failed to create image!");

		VkMemoryRequirements memRequirements{};
		vkGetImageMemoryRequirements(logicalDevice, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
			throw std::runtime_error("[Vulkan] Failed to allocate image memory!");

		if (vkBindImageMemory(logicalDevice, image, imageMemory, 0) != VK_SUCCESS)
			throw std::runtime_error("[Vulkan] Failed to bind image memory!");
	}

	VkFormat FindSupportedFormat(VkPhysicalDevice physicalDevice, const Array<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates) 
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) 
				return format;
			if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) 
				return format;
		}
		throw std::runtime_error("[Vulkan] Failed to find supported format!");
	}
}

vulkan::SwapChain::SwapChain(vulkan::Device& device, glfw::Window& window, vulkan::SwapChain* swapChain)
	: m_Device(device)
	, m_Window(window)
{
	VkFormat depthFormat = FindSupportedFormat(
		m_Device.m_PhysicalDevice,
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

	// swap-chain
	{
		VkSurfaceCapabilitiesKHR capabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_Device.m_PhysicalDevice, m_Device.m_Surface, &capabilities);

		uint32 formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_Device.m_PhysicalDevice, m_Device.m_Surface, &formatCount, nullptr);
		Array<VkSurfaceFormatKHR> formats(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_Device.m_PhysicalDevice, m_Device.m_Surface, &formatCount, formats.data());

		uint32 presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_Device.m_PhysicalDevice, m_Device.m_Surface, &presentModeCount, nullptr);
		Array<VkPresentModeKHR> presentModes(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_Device.m_PhysicalDevice, m_Device.m_Surface, &presentModeCount, presentModes.data());

		const VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(formats);
		const VkPresentModeKHR presentMode = ChooseSwapPresentMode(presentModes);
		const Vector2u windowSize = m_Window.GetSize();
		m_SwapChainExtent = ChooseSwapExtent({ windowSize.x, windowSize.y }, capabilities);
		m_SwapChainImageFormat = surfaceFormat.format;

		uint32 imageCount = capabilities.minImageCount + 1;
		if (capabilities.maxImageCount > 0)
			imageCount = std::min(imageCount, capabilities.maxImageCount);

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Device.m_Surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = m_SwapChainExtent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		const QueueFamily queueFamily = m_Device.m_QueueFamily;
		const uint32 queueFamilyIndices[] = { queueFamily.m_GraphicsFamily, queueFamily.m_PresentFamily };
		if (queueFamily.m_GraphicsFamily != queueFamily.m_PresentFamily)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		// #todo: re-create swap-chain 
		createInfo.oldSwapchain = swapChain 
			? swapChain->m_SwapChain 
			: VK_NULL_HANDLE;

		vkCreateSwapchainKHR(m_Device.m_LogicalDevice, &createInfo, nullptr, &m_SwapChain);

		if (m_SwapChain == VK_NULL_HANDLE)
			throw std::runtime_error("[Vulkan] Failed to create swap-chain!");
	}

	// render pass
	{
		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = depthFormat;
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = m_SwapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;
		subpass.inputAttachmentCount = 0;
		subpass.pInputAttachments = nullptr;
		subpass.preserveAttachmentCount = 0;
		subpass.pPreserveAttachments = nullptr;
		subpass.pResolveAttachments = nullptr;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.srcAccessMask = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstSubpass = 0;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

		Array<VkAttachmentDescription> attachments = { colorAttachment, depthAttachment };
		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		vkCreateRenderPass(m_Device.m_LogicalDevice, &renderPassInfo, nullptr, &m_RenderPass);

		if (m_RenderPass == VK_NULL_HANDLE)
			throw std::runtime_error("[Vulkan] Failed to create render pass!");
	}

	// images
	{
		uint32 swapChainImageCount;
		vkGetSwapchainImagesKHR(m_Device.m_LogicalDevice, m_SwapChain, &swapChainImageCount, nullptr);
		m_SwapChainImages.resize(swapChainImageCount);
		vkGetSwapchainImagesKHR(m_Device.m_LogicalDevice, m_SwapChain, &swapChainImageCount, m_SwapChainImages.data());

		m_SwapChainImageViews.resize(swapChainImageCount, VK_NULL_HANDLE);
		for (size_t i = 0; i < m_SwapChainImages.size(); ++i)
		{
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_SwapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_SwapChainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			vkCreateImageView(m_Device.m_LogicalDevice, &createInfo, nullptr, &m_SwapChainImageViews[i]);

			if (m_SwapChainImageViews[i] == VK_NULL_HANDLE)
				throw std::runtime_error("[Vulkan] Failed to create swap-chain image!");
		}
	}

	// depth
	{
		m_SwapChainDepthFormat = depthFormat;

		m_DepthImages.resize(m_SwapChainImages.size());
		m_DepthImageViews.resize(m_SwapChainImages.size());
		m_DepthImagesMemory.resize(m_SwapChainImages.size());

		for (int32 i = 0; i < m_DepthImages.size(); ++i)
		{
			VkImageCreateInfo imageInfo{};
			imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageInfo.imageType = VK_IMAGE_TYPE_2D;
			imageInfo.extent.width = m_SwapChainExtent.width;
			imageInfo.extent.height = m_SwapChainExtent.height;
			imageInfo.extent.depth = 1;
			imageInfo.mipLevels = 1;
			imageInfo.arrayLayers = 1;
			imageInfo.format = depthFormat;
			imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			imageInfo.flags = 0;

			CreateImageWithInfo(
				m_Device.m_LogicalDevice,
				m_Device.m_PhysicalDevice,
				imageInfo,
				m_DepthImages[i],
				m_DepthImagesMemory[i],
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

			VkImageViewCreateInfo viewInfo{};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = m_DepthImages[i];
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = depthFormat;
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.levelCount = 1;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(device.m_LogicalDevice, &viewInfo, nullptr, &m_DepthImageViews[i]) != VK_SUCCESS)
				throw std::runtime_error("[Vulkan] Failed to create texture image view!");
		}
	}

	// swap-chain frame-buffers
	m_SwapChainFramebuffers = Array<VkFramebuffer>(m_SwapChainImageViews.size(), VK_NULL_HANDLE);
	{
		for (size_t i = 0; i < m_SwapChainImageViews.size(); i++)
		{
			Array<VkImageView> attachments = { m_SwapChainImageViews[i], m_DepthImageViews[i] };

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_RenderPass;
			framebufferInfo.attachmentCount = static_cast<uint32>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = m_SwapChainExtent.width;
			framebufferInfo.height = m_SwapChainExtent.height;
			framebufferInfo.layers = 1;

			vkCreateFramebuffer(m_Device.m_LogicalDevice, &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]);

			if (m_SwapChainFramebuffers[i] == VK_NULL_HANDLE)
				throw std::runtime_error("[Vulkan] Failed to create frame-buffer!");
		}
	}

	// semaphores
	m_ImageAvailableSemaphores = Array<VkSemaphore>(s_MaxFramesInFlight, VK_NULL_HANDLE);
	m_RenderFinishedSemaphores = Array<VkSemaphore>(s_MaxFramesInFlight, VK_NULL_HANDLE);
	m_InFlightFences = Array<VkFence>(s_MaxFramesInFlight, VK_NULL_HANDLE);
	m_InFlightImages = Array<VkFence>(m_SwapChainImages.size(), VK_NULL_HANDLE);
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		for (auto& semaphore : m_ImageAvailableSemaphores)
		{
			vkCreateSemaphore(m_Device.m_LogicalDevice, &semaphoreInfo, nullptr, &semaphore);
			if (semaphore == VK_NULL_HANDLE)
				throw std::runtime_error("[Vulkan] Failed to create available semaphore!");
		}

		for (auto& semaphore : m_RenderFinishedSemaphores)
		{
			vkCreateSemaphore(m_Device.m_LogicalDevice, &semaphoreInfo, nullptr, &semaphore);
			if (semaphore == VK_NULL_HANDLE)
				throw std::runtime_error("[Vulkan] Failed to create finished semaphore!");
		}

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (auto& fence : m_InFlightFences)
		{
			vkCreateFence(m_Device.m_LogicalDevice, &fenceInfo, nullptr, &fence);
			if (fence == VK_NULL_HANDLE)
				throw std::runtime_error("[Vulkan] Failed to create fence!");
		}
	}
}

vulkan::SwapChain::~SwapChain()
{
	for (const auto& fence : m_InFlightFences)
		vkDestroyFence(m_Device.m_LogicalDevice, fence, nullptr);
	for (const auto& semaphore : m_RenderFinishedSemaphores)
		vkDestroySemaphore(m_Device.m_LogicalDevice, semaphore, nullptr);
	for (const auto& semaphore : m_ImageAvailableSemaphores)
		vkDestroySemaphore(m_Device.m_LogicalDevice, semaphore, nullptr);

	for (const auto& framebuffer : m_SwapChainFramebuffers)
		vkDestroyFramebuffer(m_Device.m_LogicalDevice, framebuffer, nullptr);

	vkDestroyRenderPass(m_Device.m_LogicalDevice, m_RenderPass, nullptr);

	for (const auto& imageView : m_DepthImageViews)
		vkDestroyImageView(m_Device.m_LogicalDevice, imageView, nullptr);
	for (const auto& image : m_DepthImages)
		vkDestroyImage(m_Device.m_LogicalDevice, image, nullptr);
	for (const auto& memory : m_DepthImagesMemory)
		vkFreeMemory(m_Device.m_LogicalDevice, memory, nullptr);

	for (const auto& imageView : m_SwapChainImageViews)
		vkDestroyImageView(m_Device.m_LogicalDevice, imageView, nullptr);

	vkDestroySwapchainKHR(m_Device.m_LogicalDevice, m_SwapChain, nullptr);
}

VkResult vulkan::SwapChain::AcquireNextImage(const uint32 frame, uint32& out_ImageIndex)
{
	const uint32 frameIndex = FrameIndex(frame);
	vkWaitForFences(m_Device.m_LogicalDevice, 1, &m_InFlightFences[frameIndex], VK_TRUE, UINT64_MAX);

	return vkAcquireNextImageKHR(
		m_Device.m_LogicalDevice,
		m_SwapChain,
		UINT64_MAX,
		m_ImageAvailableSemaphores[frameIndex],
		VK_NULL_HANDLE,
		&out_ImageIndex);
}

bool vulkan::SwapChain::HasMatchingFormats(const SwapChain& swapChain) const
{
	return swapChain.m_SwapChainDepthFormat == m_SwapChainDepthFormat
		&& swapChain.m_SwapChainImageFormat == m_SwapChainImageFormat;
}
