#include <vector>
#include <vulkan/vulkan_core.h>
namespace testVulkan {
    class VulkanInstace
    {
    public:
        VulkanInstace();
        ~VulkanInstace();

        void Destroy();
        std::vector<const char*> m_ValidationLayers;
        std::vector<const char*> m_Extensions;
        VkInstance m_VulkanInstance = VK_NULL_HANDLE;
    };
}