#pragma once

#ifdef Z_VULKAN

#include <Vulkan/vulkan.h>

namespace vulkan
{
	struct Vertex
	{
		Vector3f m_Position = { };
		Vector3f m_Colour = { };
	};

	class Device;

	class Model final
	{
	public:
		Model(vulkan::Device& device, const Array<Vertex>& vertecies, const Array<uint32>& indices = {});
		~Model();

		void Bind(VkCommandBuffer commandBuffer) const;
		void Draw(VkCommandBuffer commandBuffer) const;

	private:
		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

	private:
		vulkan::Device& m_Device;

		VkBuffer m_VertexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory m_VertexBufferMemory = VK_NULL_HANDLE;
		uint32 m_VertexCount = 0;

		VkBuffer m_IndexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory m_IndexBufferMemory = VK_NULL_HANDLE;
		uint32 m_IndexCount = 0;
	};
}

#endif