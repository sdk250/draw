#include <thread>

#include "ANativeWindowCreator.hpp"
#include "init_draw.hpp"
#include "touch.hpp"
#include "esp.hpp"

struct android::ANativeWindowCreator::DisplayInfo display_info {};
bool _shutdown = false;
uint32_t CANVAS_SIZE = 0;

int main(int argc, char **argv)
{
    puts(__TIME__ "\t" __DATE__);
    Init_draw *UI {nullptr};
    ANativeWindow *window {nullptr};
    Touch *touch {nullptr};
    std::thread *get_display_info {nullptr};
    // c_driver *driver = new c_driver((argv[1]), get_pid( /*"com.ztgame.bob.mi"*/)); // com.tencent.tmgp.pubgmhd
    ESP *esp = new ESP(argv[1], "com.tencent.tmgp.pubgmhd");

    display_info = android::ANativeWindowCreator::GetDisplayInfo();
    get_display_info = new std::thread {[&] {
        for (; !_shutdown;)
        {
            display_info = android::ANativeWindowCreator::GetDisplayInfo();
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }};
    get_display_info->detach();

    CANVAS_SIZE = display_info.width > display_info.height ? display_info.width : display_info.height;
    window = android::ANativeWindowCreator::Create("sdk250", CANVAS_SIZE, CANVAS_SIZE);
    printf("Native window address: %p\n%dx%d\tCanvas size: %d\n", window, display_info.width, display_info.height, CANVAS_SIZE);
    UI = new Init_draw(window, CANVAS_SIZE, CANVAS_SIZE);

    touch = new Touch;
    touch->start(ImGui::GetIO());

    esp->start();
    for (; !_shutdown;)
    {
        if (display_info.theta / 90 != 0)
            UI->PrepareFrame(true);
        else
            UI->PrepareFrame(false);
        ImGui_ImplAndroid_NewFrame(display_info.width, display_info.height);
        ImGui::NewFrame();

        ImGui::Begin("sdk250");

        ImGui::Text("Base address: %#lX\n", esp->libUE4);
        ImGui::Text("Fov: %.2f\tCount: %d\nUWorld: %#lX\tULevel: %#lX\n", esp->Camera, esp->Count, esp->UWorld, esp->ULevel);
        ImGui::Text("X: %.2f\tY: %.2f\tZ: %.2f\n", esp->My_pos.x, esp->My_pos.y, esp->My_pos.z);
        ImGui::Text("My term id: %d\n", esp->my_team_id);

        _shutdown = ImGui::Button("SHUTDOWN");

        ImGui::End();

        ImGui::Render();
        UI->Render(ImGui::GetDrawData());
    }
    
    delete UI;
    delete touch;
    delete get_display_info;
    delete esp;
    return 0;
}