//
// Created by ITEK on 2024/2/3.
//

#ifndef ANDROIDIMGUI_VULKANGRAPHICS_H
#define ANDROIDIMGUI_VULKANGRAPHICS_H


#include <memory>
#include "vulkan_wrapper.h"
#include "imgui_impl_vulkan.h"
#include "stb_image.h"

struct TextureInfo {
    ImTextureID textureId;
    int w;
    int h;
};

struct BaseTexData {
    void *DS = nullptr;
    int Width = 0;
    int Height = 0;
    int Channels = 0;

    BaseTexData() = default;

    BaseTexData(BaseTexData &other) = default;
};

class VulkanGraphics {
private:
    struct VulkanTextureData : BaseTexData {
        VkImageView ImageView = VK_NULL_HANDLE;
        VkImage Image = VK_NULL_HANDLE;
        VkDeviceMemory ImageMemory = VK_NULL_HANDLE;
        VkSampler Sampler = VK_NULL_HANDLE;
        VkBuffer UploadBuffer = VK_NULL_HANDLE;
        VkDeviceMemory UploadBufferMemory = VK_NULL_HANDLE;
    };

    VkAllocationCallbacks *m_Allocator = nullptr;
    VkInstance m_Instance = VK_NULL_HANDLE;
    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
    VkDevice m_Device = VK_NULL_HANDLE;
    uint32_t m_QueueFamily = (uint32_t) -1;
    VkQueue m_Queue = VK_NULL_HANDLE;
    VkDebugReportCallbackEXT m_DebugReport = VK_NULL_HANDLE;
    VkPipelineCache m_PipelineCache = VK_NULL_HANDLE;
    VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;

    std::unique_ptr<ImGui_ImplVulkanH_Window> wd{};
    int m_MinImageCount = 2;
    bool m_SwapChainRebuild = false;
    
    ANativeWindow *m_Window = nullptr;
    int m_LastWidth = 0;
    int m_LastHeight = 0;
public:
    bool Create(ANativeWindow *Window, int m_Width, int m_Height) ;

    void Setup() ;

    void PrepareFrame(bool resize) ;

    void Render(ImDrawData *drawData) ;

    void PrepareShutdown() ;

    void Cleanup() ;

    BaseTexData *LoadTexture(BaseTexData *tex_data, void *pixel_data) ;

    void RemoveTexture(BaseTexData *tex_data) ;
    
    TextureInfo ImAgeHeadFile(const unsigned char *buf, int len);

private:
    VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();

    uint32_t findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties);
};

#endif //ANDROIDIMGUI_VULKANGRAPHICS_H
