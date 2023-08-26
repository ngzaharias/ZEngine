#pragma once

#ifdef Z_VULKAN

#include <Vulkan/vulkan.h>

namespace vulkan
{
	class Device;

	struct FramebufferAttachment 
	{
		VkImage m_Image;
		VkDeviceMemory m_Memory;
		VkImageView m_View;
		VkFormat m_Format;
		VkSampler m_Sampler;
		VkDescriptorImageInfo m_Descriptor;
	};

	class Framebuffer final
	{

	private:
		Device& m_Device;

		VkFramebuffer m_Framebuffer = VK_NULL_HANDLE;
		Array<FramebufferAttachment> m_Attachments = { };
	};
}

#endif