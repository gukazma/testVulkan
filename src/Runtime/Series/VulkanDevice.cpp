#include "VulkanDevice.h"
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <vulkan/vulkan_core.h>
VulkanDevice::VulkanDevice(VkPhysicalDevice physicalDevice)
	:m_vkDevice(VK_NULL_HANDLE), m_graphicsQueue(VK_NULL_HANDLE), m_imageMemoryNeedRealloc(false)
	, m_vkdeviceInitialized(false), m_physicalDevice(physicalDevice)
{

}

VulkanDevice::~VulkanDevice()
{
	Destroy();
}

VulkanDevice* VulkanDevice::Create(VkPhysicalDevice physicalDevice, bool enableSurface, bool hasEnabledValidationLayers)
{
	auto ptrDevice = new VulkanDevice(physicalDevice);
	ptrDevice->m_hasEnabledValidationLayers = hasEnabledValidationLayers;
	if (!ptrDevice->Init(enableSurface))
		return NULL;
	return ptrDevice;
}

bool VulkanDevice::Init(bool bEnableSurface)
{
	vkGetPhysicalDeviceProperties(m_physicalDevice, &m_deviceProperties);
	vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_deviceFeatures);
	vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_memoryProperties);
    VkDeviceSize m_deviceSharedMemorySize = 0;
	m_deviceMemorySize = 0;
	for (uint32_t i = 0; i < m_memoryProperties.memoryHeapCount; ++i)
	{
		const VkMemoryHeap& heap = m_memoryProperties.memoryHeaps[i];
		const VkMemoryType& type = m_memoryProperties.memoryTypes[i];
		if (heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
		{
			m_deviceMemorySize = std::max(m_deviceMemorySize, heap.size);
		}
		if (heap.flags == 0)
		{
			m_deviceSharedMemorySize = std::max(m_deviceSharedMemorySize, heap.size);
		}
	}
	std::cout<<"Feiteng 20220510"<<std::endl;
        std::cout << "deviceMemorySize is : " << m_deviceMemorySize  << std::endl;
        std::cout << "m_deviceSharedMemorySize:" << m_deviceSharedMemorySize  << std::endl;
	//m_deviceMemorySize = std::min(m_deviceMemorySize, m_deviceSharedMemorySize); 
	//COutMsg() << "m_deviceMemorySize:" << m_deviceMemorySize  << std::endl;


	// 修正 GTX 970的显存从4GB-> 3.5 GB
	if (strstr(m_deviceProperties.deviceName, "GTX 970") != NULL)
	{
		m_deviceMemorySize = uint64_t(3.5 * 1024) * uint64_t(1024) * uint64_t(1024);
	}

	if (bEnableSurface)
		m_deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	////todo 先临时设置死最大8G
	//{
	//	VkDeviceSize maxSize = uint64_t(8 * 1024) * uint64_t(1024) * uint64_t(1024);
	//	if (m_deviceMemorySize > maxSize)
	//	{
	//		COutMsg(Warning) << "Device Memory Size is " << m_deviceMemorySize << ",we set it as :" << maxSize << std::endl;
	//		m_deviceMemorySize = maxSize;
 //		}
	//		
	//}
    //COutMsg() << "Device Memory Size is " << m_deviceMemorySize << std::endl;
	return true;
}

void VulkanDevice::Destroy()
{
	
}

int VulkanDevice::GetGraphicQueueFamily()
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, NULL);
	std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

	for (int i = 0; i < queueFamilyProperties.size(); i++)
	{
		VkQueueFamilyProperties& queueFamilyProperty = queueFamilyProperties[i];
		if (queueFamilyProperty.queueCount > 0 && queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			return i;
		}
	}
	return -1;
}

bool VulkanDevice::CreateVkDevice(int graphicQueueFamily)
{
	float const queuePriority = 1.0f;
	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = graphicQueueFamily;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = &queuePriority;


	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.occlusionQueryPrecise = true;
	deviceFeatures.geometryShader = true;
	deviceFeatures.fragmentStoresAndAtomics = true;
	deviceFeatures.fillModeNonSolid = true;
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_deviceExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = m_deviceExtensions.data();
	deviceCreateInfo.enabledLayerCount = 0;
	deviceCreateInfo.ppEnabledLayerNames= NULL;
	VkResult nErr = vkCreateDevice(m_physicalDevice, &deviceCreateInfo, NULL, &m_vkDevice);

	vkGetDeviceQueue(m_vkDevice, graphicQueueFamily, 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_vkDevice, graphicQueueFamily, 0, &m_presentQueue);
	return true;
}

uint32_t VulkanDevice::GetMemoryType(const uint32_t  typeFilter
	, const VkMemoryPropertyFlags  flags1
	, const VkMemoryPropertyFlags  flags2
	, const VkMemoryPropertyFlags  flags3)
{
	uint32_t type1 = 0;
	uint32_t type2 = 0;
	for (uint32_t i = 0; i < m_memoryProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && ((m_memoryProperties.memoryTypes[i].propertyFlags & flags1) == flags1) &&
			((m_memoryProperties.memoryTypes[i].propertyFlags & flags2) == 0))
		{
			type1 += (1 << i);
			if ((m_memoryProperties.memoryTypes[i].propertyFlags & flags3) == flags3)
			{
				type2 += (1 << i);
			}
		}
	}
	if (type2 > 0)
		return type2;
	return type1;
}

bool VulkanDevice::CalculateMemoryType()
{
	if (!CalculateDeviceMemoryType())
		return false;
	if (!CalculateImageMemoryType())
		return false;
	if (!CalculateHostMemoryType())
		return false;

	return true;
}

int VulkanDevice::GetMemoryType(int nType)
{
	int memoryType = -1;
	if (nType > 0)
	{
		VkDeviceSize maxHeap = 0;
		for (uint32_t i = 0; i < m_memoryProperties.memoryTypeCount; i++)
		{
			if (nType & (1 << i))
			{
				uint32_t  heapIndex = m_memoryProperties.memoryTypes[i].heapIndex;
				if (m_memoryProperties.memoryHeaps[heapIndex].size > maxHeap)
				{
					maxHeap = m_memoryProperties.memoryHeaps[heapIndex].size;
					memoryType = i;
std::cout<<"memoryType : "<<memoryType<<std::endl;
std::cout<<"maxHeap : "<<maxHeap<<std::endl;
				}
			}
		}
	}
	return memoryType;
}
bool VulkanDevice::CalculateHostMemoryType()
{
	VkBuffer buffer;
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.pNext = NULL;
	bufferInfo.flags = 0;
	bufferInfo.queueFamilyIndexCount = 0;
	bufferInfo.pQueueFamilyIndices = NULL;
	bufferInfo.size = (1 << 10);//1 GB

	bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	VkResult nErr = vkCreateBuffer(m_vkDevice, &bufferInfo, NULL, &buffer);

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(m_vkDevice, buffer, &memRequirements);

	VkMemoryPropertyFlags  flags1 = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	VkMemoryPropertyFlags  flags2 = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	VkMemoryPropertyFlags  flags3 = VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
	uint32_t  hostTypeBuffer = GetMemoryType(memRequirements.memoryTypeBits, flags1, flags2, flags3);
	vkDestroyBuffer(m_vkDevice, buffer, NULL);
    std::cout<<"calculate host memory type0 is : "<<hostTypeBuffer<<std::endl;
	m_hostMemoryType = GetMemoryType(hostTypeBuffer);
	std::cout<<"calculate host memory type1 is : "<<m_hostMemoryType<<std::endl;
	return true;
}

bool VulkanDevice::CalculateDeviceMemoryType()
{
	//m_deviceBufferMemoryType = 0;
	VkBuffer buffer;
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.pNext = NULL;
	bufferInfo.flags = 0;
	bufferInfo.queueFamilyIndexCount = 0;
	bufferInfo.pQueueFamilyIndices = NULL;
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = (1 << 10);//1 GB
	bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	VkResult nErr = vkCreateBuffer(m_vkDevice, &bufferInfo, NULL, &buffer);

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(m_vkDevice, buffer, &memRequirements);
	VkMemoryPropertyFlags  flags1 = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	VkMemoryPropertyFlags  flags2 = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
	VkMemoryPropertyFlags  flags3 = 0;
	uint32_t deviceTypeBuffer = GetMemoryType(memRequirements.memoryTypeBits, flags1, flags2, flags3);
	vkDestroyBuffer(m_vkDevice, buffer, NULL);

	m_deviceBufferMemoryType = GetMemoryType(deviceTypeBuffer);
	 // 如果是摩尔gpu
	if (strstr(m_deviceProperties.deviceName, "MUSA") != NULL)  
	{
		m_deviceBufferMemoryType = 1;
	}
 
   std::cout<<"calculate device memory type is : "<<m_deviceBufferMemoryType<<std::endl;
	
	return true;
}

bool VulkanDevice::CalculateImageMemoryType()
{
	VkImage image;
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = 8192;
	imageInfo.extent.height = 8192;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 10;
	imageInfo.arrayLayers = 1;
	imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;

	imageInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
	imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkResult nErr = vkCreateImage(m_vkDevice, &imageInfo, NULL, &image);
	

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(m_vkDevice, image, &memRequirements);
	VkMemoryPropertyFlags  flags1 = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	VkMemoryPropertyFlags  flags2 = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
	VkMemoryPropertyFlags  flags3 = 0;
	uint32_t deviceTypeImage = GetMemoryType(memRequirements.memoryTypeBits, flags1, flags2, flags3);
	vkDestroyImage(m_vkDevice, image, NULL);
	m_deviceImageMemoryType = GetMemoryType(deviceTypeImage);
	
	std::cout<<"calculate device image memory type is : "<<m_deviceImageMemoryType<<std::endl;
	return true;
}
void VulkanDevice::CheckImageMemory()
{
	//n卡的有些显卡，image memory需要realloc,而不是free
	if (m_deviceProperties.vendorID == 0x10de)//NVIDIA
	{
		const uint32_t deviceID = m_deviceProperties.deviceID;
		if ((deviceID >= 4032 && deviceID <= 4095) ||
			(deviceID >= 4096 && deviceID <= 4159) ||
			(deviceID >= 4480 && deviceID <= 4543) ||
			(deviceID >= 4992 && deviceID <= 5055))
		{
			m_imageMemoryNeedRealloc = true;
		}
	}
}

bool VulkanDevice::CanUseAsCompute(VkPhysicalDevice const device)
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, VK_NULL_HANDLE);

	std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties.data());

	bool bHasQueueGraphicsBit = false;
	for (auto const & queueFamilyProperty : queueFamilyProperties) {
		if (queueFamilyProperty.queueCount > 0 && queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			bHasQueueGraphicsBit = true;
			break;
		}
	}

	if (!bHasQueueGraphicsBit) {
		return false;
	}

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	if (deviceProperties.vendorID != 0x10de && deviceProperties.vendorID != 0x1002) {
		return false;
	}

	if (deviceProperties.apiVersion < VK_MAKE_VERSION(1, 0, 37)) {
		return false;
	}

	return true;
}
