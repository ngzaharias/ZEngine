#include "EnginePCH.h"
#include "Engine/Vulkan/Texture.h"

#include "Engine/Vulkan/Device.h"
#include "Engine/Vulkan/Helpers.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace
{
	void TransitionImageLayout(
		VkCommandPool commandPool, 
		VkDevice logicalDevice, 
		VkQueue graphicsQueue, 
		VkImage image, 
		VkFormat format, 
		VkImageLayout oldLayout, 
		VkImageLayout newLayout)
	{
		VkCommandBuffer commandBuffer = vulkan::BeginSingleTimeCommands(commandPool, logicalDevice);

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else 
		{
			throw std::invalid_argument("unsupported layout transition!");
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, 
			&barrier
		);

		vulkan::FinishSingleTimeCommands(commandPool, logicalDevice, graphicsQueue, commandBuffer);
	}
}

vulkan::Texture::Texture(vulkan::Device& device)
	: m_Device(device)
{
	constexpr int32 pixelSize = 4;
	int32 texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load("AssetsRaw/Textures/T_Default_Light.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

	if (!pixels)
		throw std::runtime_error("[Vulkan] Failed to load texture image!");

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	const VkDeviceSize bufferSize = texWidth * texHeight * pixelSize;

	CreateBuffer(
		m_Device.m_LogicalDevice,
		m_Device.m_PhysicalDevice,
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
		stagingBuffer, 
		stagingBufferMemory);

	void* data;
	vkMapMemory(m_Device.m_LogicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(bufferSize));
	vkUnmapMemory(m_Device.m_LogicalDevice, stagingBufferMemory);

	stbi_image_free(pixels);

	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = static_cast<uint32>(texWidth);
	imageInfo.extent.height = static_cast<uint32>(texHeight);
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

	if (vkCreateImage(m_Device.m_LogicalDevice, &imageInfo, nullptr, &m_Image) != VK_SUCCESS)
		throw std::runtime_error("[Vulkan] Failed to create image!");

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(m_Device.m_LogicalDevice, m_Image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(
		m_Device.m_PhysicalDevice, 
		memRequirements.memoryTypeBits, 
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(m_Device.m_LogicalDevice, &allocInfo, nullptr, &m_ImageMemory) != VK_SUCCESS)
		throw std::runtime_error("[Vulkan] Failed to allocate image memory!");

	vkBindImageMemory(m_Device.m_LogicalDevice, m_Image, m_ImageMemory, 0);

	TransitionImageLayout(
		m_Device.m_CommandPool,
		m_Device.m_LogicalDevice,
		m_Device.m_GraphicsQueue,
		m_Image, 
		VK_FORMAT_R8G8B8A8_SRGB, 
		VK_IMAGE_LAYOUT_UNDEFINED, 
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	// copy
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands(m_Device.m_CommandPool, m_Device.m_LogicalDevice);

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { static_cast<uint32>(texWidth), static_cast<uint32>(texHeight), 1 };

		vkCmdCopyBufferToImage(commandBuffer, stagingBuffer, m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		FinishSingleTimeCommands(
			m_Device.m_CommandPool,
			m_Device.m_LogicalDevice,
			m_Device.m_GraphicsQueue, 
			commandBuffer);
	}

	TransitionImageLayout(
		m_Device.m_CommandPool,
		m_Device.m_LogicalDevice,
		m_Device.m_GraphicsQueue,
		m_Image,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	vkDestroyBuffer(m_Device.m_LogicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(m_Device.m_LogicalDevice, stagingBufferMemory, nullptr);
}

vulkan::Texture::~Texture()
{
	vkDestroyImage(m_Device.m_LogicalDevice, m_Image, nullptr);
	vkFreeMemory(m_Device.m_LogicalDevice, m_ImageMemory, nullptr);
}