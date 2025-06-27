#include "EnginePCH.h"
#include "Engine/Vulkan/Model.h"

#include "Engine/Vulkan/Device.h"
#include "Engine/Vulkan/Helpers.h"

vulkan::Model::Model(vulkan::Device& device, const Array<Vertex>& verticies, const Array<uint32>& indices /*= {}*/)
	: m_Device(device)
	, m_VertexCount(static_cast<uint32>(verticies.size()))
	, m_IndexCount(static_cast<uint32>(indices.size()))
{
	// vertices
	{
		VkBuffer stagingBuffer = VK_NULL_HANDLE;
		VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;
		const VkDeviceSize bufferSize = sizeof(verticies[0]) * verticies.size();

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
		memcpy(data, verticies.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(m_Device.m_LogicalDevice, stagingBufferMemory);

		CreateBuffer(
			m_Device.m_LogicalDevice,
			m_Device.m_PhysicalDevice,
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_VertexBuffer,
			m_VertexBufferMemory);

		CopyBuffer(
			m_Device.m_CommandPool, 
			m_Device.m_LogicalDevice, 
			m_Device.m_GraphicsQueue, 
			stagingBuffer, 
			m_VertexBuffer, 
			bufferSize);

		vkDestroyBuffer(m_Device.m_LogicalDevice, stagingBuffer, nullptr);
		vkFreeMemory(m_Device.m_LogicalDevice, stagingBufferMemory, nullptr);
	}

	// indices
	if (!indices.empty())
	{
		VkBuffer stagingBuffer = VK_NULL_HANDLE;
		VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;
		const VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

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
		memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(m_Device.m_LogicalDevice, stagingBufferMemory);

		CreateBuffer(
			m_Device.m_LogicalDevice,
			m_Device.m_PhysicalDevice,
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_IndexBuffer,
			m_IndexBufferMemory);

		CopyBuffer(
			m_Device.m_CommandPool,
			m_Device.m_LogicalDevice,
			m_Device.m_GraphicsQueue,
			stagingBuffer,
			m_IndexBuffer,
			bufferSize);

		vkDestroyBuffer(m_Device.m_LogicalDevice, stagingBuffer, nullptr);
		vkFreeMemory(m_Device.m_LogicalDevice, stagingBufferMemory, nullptr);
	}
}

vulkan::Model::~Model()
{
	if (m_IndexBuffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(m_Device.m_LogicalDevice, m_IndexBuffer, nullptr);
		vkFreeMemory(m_Device.m_LogicalDevice, m_IndexBufferMemory, nullptr);
	}

	vkDestroyBuffer(m_Device.m_LogicalDevice, m_VertexBuffer, nullptr);
	vkFreeMemory(m_Device.m_LogicalDevice, m_VertexBufferMemory, nullptr);
}

void vulkan::Model::Bind(VkCommandBuffer commandBuffer) const
{
	// vertices
	{
		VkBuffer buffers[] = { m_VertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	// indices
	if (m_IndexBuffer != VK_NULL_HANDLE)
	{
		vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer, 0, VK_INDEX_TYPE_UINT32);
	}
}

void vulkan::Model::Draw(VkCommandBuffer commandBuffer) const
{
	if (m_IndexBuffer != VK_NULL_HANDLE)
	{
		vkCmdDrawIndexed(commandBuffer, m_IndexCount, 1, 0, 0, 0);
	}
	else
	{
		vkCmdDraw(commandBuffer, m_VertexCount, 1, 0, 0);
	}
}
