#include "EnginePCH.h"
#include "Engine/Vulkan/Helpers.h"

uint32 vulkan::FindMemoryType(VkPhysicalDevice physicalDevice, uint32 typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("[Vulkan] Failed to find suitable memory type!");
}

VkCommandBuffer vulkan::BeginSingleTimeCommands(VkCommandPool commandPool, VkDevice logicalDevice)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);
	return commandBuffer;
}

void vulkan::FinishSingleTimeCommands(
	VkCommandPool commandPool, 
	VkDevice logicalDevice, 
	VkQueue graphicsQueue, 
	VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
}

// #fixme: also used by vulkan::Texture
void vulkan::CreateBuffer(
	VkDevice logicalDevice,
	VkPhysicalDevice physicalDevice,
	VkDeviceSize size,
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties,
	VkBuffer& buffer,
	VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
		throw std::runtime_error("[Vulkan] Failed to create vertex buffer!");

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(logicalDevice, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
		throw std::runtime_error("[Vulkan] Failed to allocate vertex buffer memory!");

	vkBindBufferMemory(logicalDevice, buffer, bufferMemory, 0);
}

void vulkan::CopyBuffer(
	VkCommandPool commandPool,
	VkDevice logicalDevice,
	VkQueue graphicsQueue,
	VkBuffer srcBuffer,
	VkBuffer dstBuffer,
	VkDeviceSize size)
{
 	VkCommandBuffer commandBuffer = BeginSingleTimeCommands(commandPool, logicalDevice);

	VkBufferCopy copyBuffer{};
	copyBuffer.srcOffset = 0;
	copyBuffer.dstOffset = 0;
	copyBuffer.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyBuffer);

	FinishSingleTimeCommands(commandPool, logicalDevice, graphicsQueue, commandBuffer);
}