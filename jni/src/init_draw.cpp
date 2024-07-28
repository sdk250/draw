#include "init_draw.hpp"

Init_draw::Init_draw(ANativeWindow *_window, uint32_t width, uint32_t height): window(_window)
{
    ANativeWindow_acquire(window);
    VulkanGraphics::Create(window, width, height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplAndroid_Init(window);
    VulkanGraphics::Setup();
    ImGui::GetStyle().ScaleAllSizes(3.0f);
}

Init_draw::~Init_draw(void)
{
    VulkanGraphics::PrepareShutdown();
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();

    VulkanGraphics::Cleanup();

    ANativeWindow_release(window);
    android::ANativeWindowCreator::Destroy(window);

    window = nullptr;
    puts("Release window done!");
}