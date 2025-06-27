#pragma once

#ifdef Z_VULKAN

#include <Vulkan/vulkan.h>

namespace vulkan
{
	uint32 FindMemoryType(VkPhysicalDevice physicalDevice, uint32 typeFilter, VkMemoryPropertyFlags properties);

	VkCommandBuffer BeginSingleTimeCommands(VkCommandPool commandPool, VkDevice logicalDevice);
	void FinishSingleTimeCommands(
		VkCommandPool commandPool, 
		VkDevice logicalDevice, 
		VkQueue graphicsQueue, 
		VkCommandBuffer commandBuffer);

	void CreateBuffer(
		VkDevice logicalDevice,
		VkPhysicalDevice physicalDevice,
		VkDeviceSize size,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkBuffer& buffer,
		VkDeviceMemory& bufferMemory);

	void CopyBuffer(
		VkCommandPool commandPool,
		VkDevice logicalDevice,
		VkQueue graphicsQueue,
		VkBuffer srcBuffer,
		VkBuffer dstBuffer,
		VkDeviceSize size);
}

#endif