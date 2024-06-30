#include <iostream>
#include <thread>
#include <dirent.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <cassert>
#include <unistd.h>
#include <linux/input.h>
#include <fcntl.h>
#include "ANativeWindowCreator.hpp"
#include "imgui.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"
#include "wanbai.hpp"

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) (( ((x) - 1) / BITS_PER_LONG) + 1)
#define CBIT(ARRAY, BIT) (\
    (\
        ARRAY[BIT / BITS_PER_LONG] >> BIT % BITS_PER_LONG\
    ) & 1\
)

int get_input_device(const struct dirent *dir);
const char* exec(const char* command);
pid_t get_pid(const char* package);

int main(int argc, char **argv)
{
    std::cout << __TIME__ << '\t' << __DATE__ << std::endl;
    if (argc < 2)
        return 2;

    ANativeWindow *native_window = nullptr;
    EGLDisplay eDisplay = EGL_NO_DISPLAY;
    EGLSurface eSurface = EGL_NO_SURFACE;
    EGLContext eContext = EGL_NO_CONTEXT;
    EGLint eAttributes[] = {
        EGL_BUFFER_SIZE, 32,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 16,
        EGL_STENCIL_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    }, num_config = 0, eFormat = 0;
    EGLConfig eConfig = nullptr;
    // ImGuiIO& io = ImGui::GetIO();
    // static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool show_demo_window = true, _shutdown = false;
    int ndev = -1;
    struct dirent **namelist {nullptr};
    char input_device[30] {0};
    std::thread *pth {nullptr}, *d_info {nullptr};
    struct input_event touch_event;
    struct android::ANativeWindowCreator::DisplayInfo dInfo {0};
    c_driver *driver = new c_driver(strdup(argv[1]), get_pid("com.ztgame.bob.mi")); // com.tencent.tmgp.pubgmhd
    uint32_t libUE4 = 0;

    d_info = new std::thread {[&] {
        for (; !_shutdown;)
        {
            dInfo = android::ANativeWindowCreator::GetDisplayInfo();
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }};
    d_info->detach();
    sleep(2);

    native_window = android::ANativeWindowCreator::Create(
        "sdk250",
        dInfo.width + dInfo.height,
        dInfo.height + dInfo.width
    );

    ANativeWindow_acquire(native_window);

    printf("Android native window: %8p\n", native_window);

    /* Begin initialize EGL. */
    eDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assert(eDisplay != EGL_NO_DISPLAY);
    eglInitialize(eDisplay, nullptr, nullptr);
    eglGetConfigs(eDisplay, nullptr, (EGLint) 1, &num_config);
    eglChooseConfig(eDisplay, eAttributes, &eConfig, (EGLint) 1, &num_config);
    printf("num config: %d\n", (int) num_config);
    eglGetConfigAttrib(eDisplay, eConfig, EGL_NATIVE_VISUAL_ID, &eFormat);
    printf("EGL format: %d\n", eFormat);
    ANativeWindow_setBuffersGeometry(native_window, 0, 0, eFormat);
    eContext = eglCreateContext(eDisplay, eConfig, EGL_NO_CONTEXT, (const EGLint []) {
        EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE
    });
    assert(eContext != EGL_NO_CONTEXT);
    eSurface = eglCreateWindowSurface(eDisplay, eConfig, native_window, nullptr);
    assert(eSurface != EGL_NO_SURFACE);
    eglMakeCurrent(eDisplay, eSurface, eSurface, eContext);
    /* EGL Context initialization is complete. */

    // printf("%d x %d\n", ImGui::GetIO().DisplaySize.x, ANativeWindow_getHeight(native_window));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().IniFilename = nullptr;
    ImGui::GetIO().Fonts->AddFontFromFileTTF(
        "/storage/emulated/0/item/Fonts/Heiti.ttf",
        40.0f,
        nullptr,
        ImGui::GetIO().Fonts->GetGlyphRangesChineseSimplifiedCommon()
    );
    ImGui::StyleColorsDark();
    ImGui_ImplAndroid_Init(native_window);
    ImGui_ImplOpenGL3_Init("#version 300 es");
    // ImGui::GetStyle().ScaleAllSizes(10.0f);

    ndev = scandir("/dev/input", &namelist, get_input_device, alphasort);
    for(int i = 0, fd = -1; i++ < ndev;)
    {
        unsigned long keybit[NBITS(KEY_CNT)];
        snprintf(input_device, sizeof(input_device), "%s/%s", "/dev/input", namelist[i]->d_name);
        if((fd = open(input_device, O_RDONLY)) < 0)
        {
            free(namelist[i]);
            continue;
        }
        memset(keybit, 0, sizeof(keybit));
        ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keybit)), keybit);
        close(fd);
        free(namelist[i]);
        if (CBIT(keybit, BTN_TOUCH) || CBIT(keybit, BTN_TOOL_FINGER))
            break;
    }
    puts(input_device);
    int fd = open(input_device, O_RDONLY | O_SYNC | O_NONBLOCK);
    float x, y, z;
    assert(fd > 0);
    ImGuiIO &io = ImGui::GetIO();
    pth = new std::thread {[&]{
        for (; !_shutdown;)
        {
            if (read(fd, &touch_event, sizeof(touch_event)) > 0)
            {
                switch(touch_event.code)
                {
                    case ABS_MT_POSITION_X:
                        x = touch_event.value / 100;
                        continue;
                    case ABS_MT_POSITION_Y:
                        y = touch_event.value / 100;
                        continue;
                    case ABS_MT_TRACKING_ID:
                        z = touch_event.value;
                    default:
                        break;
                }
                // printf("x: %f\ty: %f\tz: %d\n", ImVec2(x,y)[0], ImVec2(x,y)[1], dInfo.theta);
                z < 0 ? io.MouseDown[0] = false : io.MouseDown[0] = true;
                switch (dInfo.theta / 90)
                {
                    case 0:
                        io.MousePos = ImVec2(x, y);
                        continue;
                    case 1:
                        io.MousePos = ImVec2(y , dInfo.height - x);
                        continue;
                    case 3:
                        io.MousePos = ImVec2(dInfo.width - y, x);
                    default:
                        break;
                }
            }
        }
    }};
    pth->detach();
    bool check = false;

    for (; !_shutdown; )
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplAndroid_NewFrame(dInfo.width + dInfo.height, dInfo.width + dInfo.height);
        ImGui::NewFrame();

        ImGui::ShowDemoWindow(&show_demo_window);
        ImGui::Begin("sdk250"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("demo window", &show_demo_window);

        check = ImGui::Button("Check");
        if (check)
            libUE4 = driver->getModuleBase("libunity.so");
        ImGui::Text("Base address: %#x", libUE4);

        _shutdown = ImGui::Button("close");
        ImGui::End();

        ImGui::Render();
        glViewport(0, 0, (int) dInfo.width + dInfo.height, (int) dInfo.width + dInfo.height);
        glClearColor(0,0,0,0/*clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w*/);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        eglSwapBuffers(eDisplay, eSurface);
    }

    puts("RELEASE WINDOW!");
    delete pth;
    delete d_info;
    delete driver;
    close(fd);
    ImGui_ImplAndroid_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    eContext != EGL_NO_CONTEXT ? eglDestroyContext(eDisplay, eContext) : 0x0;
    eSurface != EGL_NO_SURFACE ? eglDestroySurface(eDisplay, eSurface) : 0x0;
    eglTerminate(eDisplay);
    ANativeWindow_release(native_window);
    android::ANativeWindowCreator::Destroy(native_window);
    native_window = nullptr;

    return 0;
}

int get_input_device(const struct dirent *dir)
{
    return !strncmp("event", dir->d_name, 5);
}

pid_t get_pid(const char* package)
{
    char command[69];
    int pid = -1;
    sprintf(command, "pidof %s", package);
    pid = atoi(exec(command));
    if (pid < 10)
    {
        return -1;
        // DIR* dir = NULL;
        // struct dirent* ptr = NULL;
        // FILE* fp = NULL;
        // char filepath[256];
        // char filetext[128];
        // dir = opendir("/proc");
        // if (NULL != dir)
        // {
        //     while ((ptr = readdir(dir)) != NULL)
        //     {
        //         if ((strcmp(ptr->d_name, ".") == 0) ||
        //             (strcmp(ptr->d_name, "..") == 0) ||
        //             ptr->d_type != DT_DIR
        //         )
        //             continue;
        //         sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);
        //         fp = fopen(filepath, "r");
        //         if (NULL != fp)
        //         {
        //             fgets(filetext, sizeof(filetext), fp);
        //             if (strcmp(filetext, package) == 0)
        //             {
        //                 pid = atoi(ptr->d_name);
        //                 fclose(fp);
        //                 break;
        //             }
        //             fclose(fp);
        //         }
        //     }
        //     closedir(dir);
        // }
    }
    return pid;
}

const char* exec(const char* command)
{
    static char result[100 * 32];
    memset(result, '\0', sizeof(result));
    // Open pipe to file
    FILE* pipe = popen(command, "r");
    if (!pipe)
        return "popen failed!";
    // read till end of process:
    if (!fgets(result, 100 * 32, pipe))
        perror("Read command");
    pclose(pipe);
    return (const char*) result;
}

