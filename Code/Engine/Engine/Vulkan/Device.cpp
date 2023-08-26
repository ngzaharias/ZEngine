#include "EnginePCH.h"
#include "Engine/Vulkan/Device.h"

#include "Engine/GLFW/Window.h"

#include <iostream>

namespace
{
	constexpr float s_QueuePriority = 1.f;
	const Array<const char*> s_DeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	const Array<const char*> s_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) 
	{
		std::cerr << "[Vulkan] " << pCallbackData->pMessage << '\n';
		return VK_FALSE;
	}

	bool HasRequiredExtensions(const VkPhysicalDevice& device)
	{
		uint32 extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
		Array<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		Set<str::String> missingExtensions(s_DeviceExtensions.begin(), s_DeviceExtensions.end());
		for (const auto& extension : availableExtensions)
			missingExtensions.erase(extension.extensionName);
		return missingExtensions.empty();
	};

	bool IsValidationLayerSupported()
	{
		uint32 layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		Array<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : s_ValidationLayers)
		{
			bool layerFound = false;
			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
				return false;
		}

		return true;
	}

	vulkan::QueueFamily FindQueueFamily(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
	{
		vulkan::QueueFamily result;

		uint32 count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
		Array<VkQueueFamilyProperties> values(count);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &count, values.data());

		uint32 i = 0;
		for (const auto& queueFamily : values)
		{
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				result.m_GraphicsFamily = i;

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
			if (queueFamily.queueCount > 0 && presentSupport)
				result.m_PresentFamily = i;

			if (result.m_GraphicsFamily && result.m_PresentFamily)
				break;
			i++;
		}
		return result;
	}
}

vulkan::Device::Device(glfw::Window& window)
	: m_Window(window)
{
#ifdef Z_VULKAN_VALIDATION_LAYERS
	if (!IsValidationLayerSupported())
		return;
#endif


#ifdef Z_GLFW
	//////////////////////////////////////////////////////////////////////////
	// GLFW Extensions

	const auto requiredExtensions = m_Window.GetRequiredExtensions();
	{
		uint32 extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		Array<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		Set<str::String> availableExtensions;
		for (const auto& extension : extensions)
			availableExtensions.insert(extension.extensionName);

		for (const auto& extension : requiredExtensions)
		{
			if (availableExtensions.find(extension) == availableExtensions.end())
				throw std::runtime_error("[Vulkan] Missing required glfw extension!");
		}
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Instance
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "ZEngine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCreateInfo.messageSeverity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugCreateInfo.messageType =
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugCreateInfo.pfnUserCallback = DebugCallback;
		debugCreateInfo.pUserData = nullptr;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
		createInfo.ppEnabledExtensionNames = requiredExtensions.data();
#ifdef Z_VULKAN_VALIDATION_LAYERS
		createInfo.enabledLayerCount = static_cast<uint32_t>(s_ValidationLayers.size());
		createInfo.ppEnabledLayerNames = s_ValidationLayers.data();
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
#endif

		vkCreateInstance(&createInfo, nullptr, &m_Instance);

		if (m_Instance == VK_NULL_HANDLE)
			throw std::runtime_error("[Vulkan] Failed to create an instance!");
	}

	//////////////////////////////////////////////////////////////////////////
	// Debug Messenger
#ifdef Z_VULKAN_VALIDATION_LAYERS
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = 
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = 
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;
		createInfo.pUserData = nullptr;

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");
		const auto result = func
			? func(m_Instance, &createInfo, nullptr, &m_DebugMessenger)
			: VK_ERROR_EXTENSION_NOT_PRESENT;

		if (result != VK_SUCCESS) 
			throw std::runtime_error("failed to set up debug messenger!");
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Surface
	{
		m_Window.CreateWindowSurface(m_Instance, m_Surface);
	}

	//////////////////////////////////////////////////////////////////////////
	// Physical Device
	{
		uint32 deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
		Array<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

		for (const VkPhysicalDevice& device : devices)
		{
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

			if (deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				continue;
			if (!deviceFeatures.geometryShader)
				continue;

			m_QueueFamily = FindQueueFamily(device, m_Surface);
			if (m_QueueFamily.m_GraphicsFamily == UINT_MAX || m_QueueFamily.m_PresentFamily == UINT_MAX)
				continue;

			if (!HasRequiredExtensions(device))
				continue;

			uint32 formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, nullptr);
			Array<VkSurfaceFormatKHR> formats(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, formats.data());
			if (formats.empty())
				continue;

			uint32 presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, nullptr);
			Array<VkPresentModeKHR> presentModes(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, presentModes.data());
			if (presentModes.empty())
				continue;

			// #todo: pick the device with the best score
			m_PhysicalDevice = device;
			break;
		}

		if (m_PhysicalDevice == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to select a physical device!");

		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &properties);
		std::cout << "[Vulkan] Physical Device: " << properties.deviceName << '\n';
	}

	const QueueFamily queueFamily = FindQueueFamily(m_PhysicalDevice, m_Surface);

	//////////////////////////////////////////////////////////////////////////
	// Logical Device
	{
		const Set<uint32> queueFamilies = { m_QueueFamily.m_GraphicsFamily, m_QueueFamily.m_PresentFamily };

		Array<VkDeviceQueueCreateInfo> queueCreateInfos;
		for (uint32 queueFamilyIndex : queueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &s_QueuePriority;

			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.alphaToOne = VK_FALSE;
		deviceFeatures.depthBiasClamp = VK_FALSE;
		deviceFeatures.depthBounds = VK_FALSE;
		deviceFeatures.depthClamp = VK_FALSE;
		deviceFeatures.drawIndirectFirstInstance = VK_FALSE;
		deviceFeatures.dualSrcBlend = VK_FALSE;
		deviceFeatures.fillModeNonSolid = VK_FALSE;
		deviceFeatures.fragmentStoresAndAtomics = VK_FALSE;
		deviceFeatures.fullDrawIndexUint32 = VK_FALSE;
		deviceFeatures.geometryShader = VK_FALSE;
		deviceFeatures.imageCubeArray = VK_FALSE;
		deviceFeatures.independentBlend = VK_FALSE;
		deviceFeatures.inheritedQueries = VK_FALSE;
		deviceFeatures.largePoints = VK_FALSE;
		deviceFeatures.logicOp = VK_FALSE;
		deviceFeatures.multiDrawIndirect = VK_FALSE;
		deviceFeatures.multiViewport = VK_FALSE;
		deviceFeatures.occlusionQueryPrecise = VK_FALSE;
		deviceFeatures.pipelineStatisticsQuery = VK_FALSE;
		deviceFeatures.robustBufferAccess = VK_FALSE;
		deviceFeatures.samplerAnisotropy = VK_FALSE;
		deviceFeatures.sampleRateShading = VK_FALSE;
		deviceFeatures.shaderClipDistance = VK_FALSE;
		deviceFeatures.shaderCullDistance = VK_FALSE;
		deviceFeatures.shaderFloat64 = VK_FALSE;
		deviceFeatures.shaderImageGatherExtended = VK_FALSE;
		deviceFeatures.shaderInt16 = VK_FALSE;
		deviceFeatures.shaderInt64 = VK_FALSE;
		deviceFeatures.shaderResourceMinLod = VK_FALSE;
		deviceFeatures.shaderResourceResidency = VK_FALSE;
		deviceFeatures.shaderSampledImageArrayDynamicIndexing = VK_FALSE;
		deviceFeatures.shaderStorageBufferArrayDynamicIndexing = VK_FALSE;
		deviceFeatures.shaderStorageImageArrayDynamicIndexing = VK_FALSE;
		deviceFeatures.shaderStorageImageExtendedFormats = VK_FALSE;
		deviceFeatures.shaderStorageImageMultisample = VK_FALSE;
		deviceFeatures.shaderStorageImageReadWithoutFormat = VK_FALSE;
		deviceFeatures.shaderStorageImageWriteWithoutFormat = VK_FALSE;
		deviceFeatures.shaderTessellationAndGeometryPointSize = VK_FALSE;
		deviceFeatures.shaderUniformBufferArrayDynamicIndexing = VK_FALSE;
		deviceFeatures.sparseBinding = VK_FALSE;
		deviceFeatures.sparseResidency16Samples = VK_FALSE;
		deviceFeatures.sparseResidency2Samples = VK_FALSE;
		deviceFeatures.sparseResidency4Samples = VK_FALSE;
		deviceFeatures.sparseResidency8Samples = VK_FALSE;
		deviceFeatures.sparseResidencyAliased = VK_FALSE;
		deviceFeatures.sparseResidencyBuffer = VK_FALSE;
		deviceFeatures.sparseResidencyImage2D = VK_FALSE;
		deviceFeatures.sparseResidencyImage3D = VK_FALSE;
		deviceFeatures.tessellationShader = VK_FALSE;
		deviceFeatures.textureCompressionASTC_LDR = VK_FALSE;
		deviceFeatures.textureCompressionBC = VK_FALSE;
		deviceFeatures.textureCompressionETC2 = VK_FALSE;
		deviceFeatures.variableMultisampleRate = VK_FALSE;
		deviceFeatures.vertexPipelineStoresAndAtomics = VK_FALSE;
		deviceFeatures.wideLines = VK_FALSE;

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(s_DeviceExtensions.size());
		createInfo.ppEnabledExtensionNames = s_DeviceExtensions.data();
#ifdef Z_VULKAN_VALIDATION_LAYERS
		createInfo.enabledLayerCount = static_cast<uint32_t>(s_ValidationLayers.size());
		createInfo.ppEnabledLayerNames = s_ValidationLayers.data();
#endif

		vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_LogicalDevice);

		if (m_LogicalDevice == VK_NULL_HANDLE)
			throw std::runtime_error("[Vulkan] Failed to create a logical device!");

		//////////////////////////////////////////////////////////////////////////
		// Queues
		{
			vkGetDeviceQueue(m_LogicalDevice, m_QueueFamily.m_GraphicsFamily, 0, &m_GraphicsQueue);
			vkGetDeviceQueue(m_LogicalDevice, m_QueueFamily.m_PresentFamily, 0, &m_PresentQueue);

			if (m_GraphicsQueue == VK_NULL_HANDLE)
				throw std::runtime_error("[Vulkan] Failed to get graphics queue!");
			if (m_PresentQueue == VK_NULL_HANDLE)
				throw std::runtime_error("[Vulkan] Failed to get present queue!");
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Command Pool
	{
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = m_QueueFamily.m_GraphicsFamily;
		poolInfo.flags = 
			VK_COMMAND_POOL_CREATE_TRANSIENT_BIT |
			VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		vkCreateCommandPool(m_LogicalDevice, &poolInfo, nullptr, &m_CommandPool);

		if (m_CommandPool == VK_NULL_HANDLE)
			throw std::runtime_error("[Vulkan] Failed to create command pool!");
	}
}

vulkan::Device::~Device()
{
	vkDestroyCommandPool(m_LogicalDevice, m_CommandPool, nullptr);
	vkDestroyDevice(m_LogicalDevice, nullptr);
	vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);

	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func) 
		func(m_Instance, m_DebugMessenger, nullptr);

	vkDestroyInstance(m_Instance, nullptr);
}