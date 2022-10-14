#include <vector>
#include <vulkan/vulkan_core.h>
namespace testVulkan {
    class VulkanInstace
    {
    public:
        static VulkanInstace* Create();

    private:
        std::vector<const char*> m_ValidationLayers;
        std::vector<const char*> m_Extensions;
        VkInstance m_VulkanInstance;
        VulkanInstace();

        ~VulkanInstace();
    };
}