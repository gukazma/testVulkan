#include <vulkan/vulkan.h>
#include <vector>

class VulkanDevice
{
	friend class VulkanCommandBufferPool;
	friend class VulkanCommandBuffer;
	friend class VulkanMemory;
	friend class VulkanImage;
	friend class VulkanProgram;
	friend class VulkanUniform;
protected:
	VulkanDevice(VkPhysicalDevice physicalDevice);
	~VulkanDevice();
public:
	static VulkanDevice* Create(VkPhysicalDevice, bool enableSurface,bool hasEnabledValidationLayers);

	static bool CanUseAsCompute(VkPhysicalDevice const device);
	bool Init(bool bEnableSurface);
	void Destroy();
	int GetGraphicQueueFamily();

	bool CreateVkDevice(int graphicQueueFamily);

	bool InitializeVKDevice();

	bool CalculateMemoryType();

	uint32_t GetMemoryType(const uint32_t  typeFilter
		, const VkMemoryPropertyFlags  flags1
		, const VkMemoryPropertyFlags  flags2
		, const VkMemoryPropertyFlags  flags3);

	bool CalculateDeviceMemoryType();

	bool CalculateImageMemoryType();

	bool CalculateHostMemoryType();

	int GetMemoryType(int nType);
	void CheckImageMemory();

public:
	VkPhysicalDevice m_physicalDevice;
	VkPhysicalDeviceProperties m_deviceProperties;
	VkPhysicalDeviceFeatures m_deviceFeatures;
	VkPhysicalDeviceMemoryProperties m_memoryProperties;
	VkDeviceSize m_deviceMemorySize;
	bool m_vkdeviceInitialized;
	bool m_imageMemoryNeedRealloc;
	bool m_hasEnabledValidationLayers;
	std::vector<const char*> m_deviceExtensions;
	VkDevice m_vkDevice;
	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;
	VkCommandPool m_commandPool;
	VkCommandPool m_transientCommandPool;
	int m_deviceBufferMemoryType = -1;
	int m_deviceImageMemoryType = -1;
	int m_hostMemoryType = -1;
};