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

struct Vec2 {
    float x;
    float y;
    Vec2()
    {
        this->x = 0;
        this->y = 0;
    }
    Vec2(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
    Vec2 operator+(float v) const
    {
        return Vec2(x + v, y + v);
    }
    Vec2 operator-(float v) const
    {
        return Vec2(x - v, y - v);
    }
    Vec2 operator*(float v) const
    {
        return Vec2(x * v, y * v);
    }
    Vec2 operator/(float v) const
    {
        return Vec2(x / v, y / v);
    }
    Vec2& operator+=(float v)
    {
        x += v;
        y += v;
        return *this;
    }
    Vec2& operator-=(float v)
    {
        x -= v;
        y -= v;
        return *this;
    }
    Vec2& operator*=(float v)
    {
        x *= v;
        y *= v;
        return *this;
    }
    Vec2& operator/=(float v)
    {
        x /= v;
        y /= v;
        return *this;
    }
    Vec2 operator+(const Vec2& v) const
    {
        return Vec2(x + v.x, y + v.y);
    }
    Vec2 operator-(const Vec2& v) const
    {
        return Vec2(x - v.x, y - v.y);
    }
    Vec2 operator*(const Vec2& v) const
    {
        return Vec2(x * v.x, y * v.y);
    }
    Vec2 operator/(const Vec2& v) const
    {
        return Vec2(x / v.x, y / v.y);
    }
    Vec2& operator+=(const Vec2& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }
    Vec2& operator-=(const Vec2& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    Vec2& operator*=(const Vec2& v)
    {
        x *= v.x;
        y *= v.y;
        return *this;
    }
    Vec2& operator/=(const Vec2& v)
    {
        x /= v.x;
        y /= v.y;
        return *this;
    }
    operator ImVec2() const
    {
        return ImVec2(this->x, this->y);
    }
};

struct Vec3
{
    float x;
    float y;
    float z;
    Vec3()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
    Vec3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vec3 operator+(float v) const {
        return Vec3(x + v, y + v, z + v);
    }
    Vec3 operator-(float v) const {
        return Vec3(x - v, y - v, z - v);
    }
    Vec3 operator*(float v) const {
        return Vec3(x * v, y * v, z * v);
    }
    Vec3 operator/(float v) const {
        return Vec3(x / v, y / v, z / v);
    }
    Vec3& operator+=(float v)
    {
        x += v;
        y += v;
        z += v;
        return *this;
    }
    Vec3& operator-=(float v)
    {
        x -= v;
        y -= v;
        z -= v;
        return *this;
    }
    Vec3& operator*=(float v)
    {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }
    Vec3& operator/=(float v)
    {
        x /= v;
        y /= v;
        z /= v;
        return *this;
    }
    Vec3 operator+(const Vec3& v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }
    Vec3 operator-(const Vec3& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }
    Vec3 operator*(const Vec3& v) const {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }
    Vec3 operator/(const Vec3& v) const {
        return Vec3(x / v.x, y / v.y, z / v.z);
    }
    Vec3& operator+=(const Vec3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vec3& operator-=(const Vec3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    Vec3& operator*=(const Vec3& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    Vec3& operator/=(const Vec3& v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }
};

struct Player
{
    float Health {0};
    struct {
        struct Vec3 Position {0, 0, 0};
        struct Vec2 Screen_Position {0, 0};
    } Position,
        Head,
        Chest,
        Pelvis,
        lSh,
        rSh,
        lElbow,
        rElbow,
        lWrist,
        rWrist,
        lLeg,
        rLeg,
        lKnee,
        rKnee,
        lAnkle,
        rAnkle;
    int TeamID {0};
    bool Bot {false};
    float Distance {0};
    float Camera {0};
    float Width {0};
};

struct FMatrix
{
    float M[4][4];
};

struct Quat
{
    float x;
    float y;
    float z;
    float w;
};

struct FTransform
{
    Quat Rotation;
    Vec3 Translation;
    float chunk;
    Vec3 Scale3D;
};


int get_input_device(const struct dirent *dir);
const char* exec(const char* command);
pid_t get_pid(const char* package);
struct FMatrix TransformToMatrix(const FTransform transform);
void WorldToScreen(Vec2 *bscreen, float *camea, float *w, const Vec3 obj, const float *matrix, int , int );
FMatrix MatrixMulti(const FMatrix m1, const FMatrix m2);
inline Vec3 MarixToVector(const FMatrix matrix);
Vec2 WorldToScreen(const Vec3 obj, const float *matrix, const int Width, const int Height);
void signal_terminate(int sign);
inline struct FTransform getBone(uintptr_t addr, c_driver *driver);

static bool _shutdown = false;
// Players
static struct Player players[200];
const uint32_t RED = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 0.0f, 0.0f, 255.0f));
const uint32_t GREEN = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 255.0f, 0.0f, 255.0f));
const uint32_t BLUE = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 255.0f, 255.0f));
struct android::ANativeWindowCreator::DisplayInfo dInfo {0};
const float line_width = 1.5f;

int main(int argc, char **argv)
{
    std::cout << __TIME__ << '\t' << __DATE__ << std::endl;
    if (argc < 2)
        return 2;

    signal(SIGTERM, signal_terminate);

    // Android native window
    ANativeWindow *native_window {nullptr};

    // Initialize EGL
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

    // Demo window and shutdown comfirm
    bool show_demo_window = true;

    // Scan touch device
    int ndev = -1;
    struct dirent **namelist {nullptr};
    char input_device[30] {0};
    struct input_event touch_event;

    // Thread pointer
    std::thread *pth {nullptr}, *d_info {nullptr}, *game_data {nullptr};

    // Driver
    c_driver *driver = new c_driver((argv[1]), get_pid("com.tencent.tmgp.pubgmhd" /*"com.ztgame.bob.mi"*/)); // com.tencent.tmgp.pubgmhd

    // Game data
    uintptr_t libUE4 = 0;
    libUE4 = driver->getModuleBase("libUE4.so");

    d_info = new std::thread {[&] {
        for (; !_shutdown;)
        {
            dInfo = android::ANativeWindowCreator::GetDisplayInfo();
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }};
    d_info->detach();
    sleep(2);

    int32_t w_h = dInfo.width + dInfo.height;
    printf("Pixel: %d\n", w_h);
    native_window = android::ANativeWindowCreator::Create(
        "sdk250",
        w_h,
        w_h
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
    // ImGuiIO &io = ImGui::GetIO();
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
                z < 0 ? ImGui::GetIO().MouseDown[0] = false : ImGui::GetIO().MouseDown[0] = true;
                switch (dInfo.theta / 90)
                {
                    case 0:
                        ImGui::GetIO().MousePos = ImVec2(x, y);
                        continue;
                    case 1:
                        ImGui::GetIO().MousePos = ImVec2(y , dInfo.height - x);
                        continue;
                    case 3:
                        ImGui::GetIO().MousePos = ImVec2(dInfo.width - y, x);
                    default:
                        break;
                }
            }
        }
    }};
    pth->detach();
    bool check = false;
    uintptr_t value {0};
    uintptr_t offset {0};

    uintptr_t UWorld = 0, ULevel = 0, Array = 0, Matrix = 0, Self = 0, Mesh = 0, Bone = 0, Object = 0;
    int Count = 0, my_team_id = 0;
    struct Vec3 My_pos;
    float matrix_content[16] {0.0f}, Fov {0.0f}, Camera {0.0f};
    // ImDrawList *drawList = ImGui::GetForegroundDrawList();

    game_data = new std::thread {[&]{
        // for (; !_shutdown;)
        // {

        // }
    }};
    game_data->detach();

    bool _1 = false;
    offset = 0x2F40;
    for (; !_shutdown; )
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplAndroid_NewFrame(w_h, w_h);
        ImGui::NewFrame();

        ImGui::Begin("sdk250");

        UWorld = driver->read<uintptr_t>(libUE4 + 0xE9985C8);
        ULevel = driver->read<uintptr_t>(UWorld + 0x90);
        Array = driver->read<uintptr_t>(ULevel + 0xA0);
        Count = driver->read<int>(ULevel + 0xA8);

        if (Count < 1 || Count > 2000)
            goto end;

        Matrix = driver->read<uintptr_t>(
            driver->read<uintptr_t>(
                libUE4 + 0xE968E88
            ) + 0x20
        ) + 0x270;
        Self = driver->read<uintptr_t>(
            driver->read<uintptr_t>(
                driver->read<uintptr_t>(
                    driver->read<uintptr_t>(
                        UWorld + 0x98
                    ) + 0x88
                ) + 0x30
            ) + 0x2E80
        );
        my_team_id = driver->read<int>(Self + 0xA80);
        driver->read(driver->read<uintptr_t>(Self + 0x268) + 0x1C0, &My_pos, sizeof(struct Vec3));
        driver->read(Matrix, &matrix_content, sizeof(matrix_content));
        Fov = driver->read<float>(
            driver->read<uintptr_t>(
                driver->read<uintptr_t>(
                    Self + 0x4A70
                ) + 0x5D0
            ) + 0x5F0
        );

        ImGui::Text("Version 0.3");

        check = ImGui::Button("+1__");
        _1 = ImGui::Button("-1__");
        if (check)
        {
            // libUE4 = driver->getModuleBase("libUE4.so");
            // value = driver->read<uintptr_t>(libUE4 + offset);
            offset += 1;
        }
        if (_1) offset -=1;
        ImGui::Text("Base address: %#lX\nValue: %lu\n", libUE4, value);
        ImGui::Text("Fov: %.2f\tCount: %d\nUWorld: %#lX\tULevel: %#lX\n", Camera, Count, UWorld, ULevel);
        ImGui::Text("X: %.2f\tY: %.2f\tZ: %.2f\n", My_pos.x, My_pos.y, My_pos.z);
        ImGui::Text("My term id: %d\n", my_team_id);
        // drawList = ImGui::GetForegroundDrawList();

        for (int i = 0, j = 0; i < Count && j < 100; i++)
        {
            Object = driver->read<uintptr_t>(Array + i * sizeof(uintptr_t));

            if (Object <= 0xFFFF || Object == 0 || Object <= 0x10000000 || Object % 4 != 0 || Object >= 0x10000000000)
                continue;

            uintptr_t object = driver->read<uintptr_t>(Object + 0x268);

            if (object <= 0xFFFF || object == 0 || object <= 0x10000000 || object % 4 != 0 || object >= 0x10000000000)
                continue;

            if (driver->read<float>(Object + 0x2F40) != 479.5f)
                continue;

            driver->read(object + 0x1C0, &players[j].Position.Position, sizeof(struct Vec3));
            if (players[j].Position.Position.x == 0.0f || players[j].Position.Position.y == 0.0f)
                continue;

            players[j].TeamID = driver->read<int>(Object + 0xA80);
            if (players[j].TeamID == my_team_id)
                continue;

            int state = driver->read<int>(Object + 0x1328);
            if (state == 0x100000 || state == 0x100010)
                continue;

            players[j].Health = (driver->read<float>(Object + 0xDF8) / driver->read<float>(Object + 0xE00)) * 100;
            if (players[j].Health > 100 || players[j].Health <= 0)
                continue;

            // Camera = matrix_content[3] * players[j].Position.Position.x + matrix_content[7] * players[j].Position.Position.y + matrix_content[11] * players[j].Position.Position.z + matrix_content[15];
            // float r_x = (dInfo.width / 2) + (matrix_content[0] * players[j].Position.Position.x + matrix_content[4] * players[j].Position.Position.y + matrix_content[8] * players[j].Position.Position.z + matrix_content[12]) / Camera * (dInfo.width / 2);
            // float r_y = (dInfo.height / 2) - (matrix_content[1] * players[j].Position.Position.x + matrix_content[5] * players[j].Position.Position.y + matrix_content[9] * (players[j].Position.Position.z - 5) + matrix_content[13]) / Camera * (dInfo.height / 2);
            // float r_w = (dInfo.height / 2) - (matrix_content[1] * players[j].Position.Position.x + matrix_content[5] * players[j].Position.Position.y + matrix_content[9] * (players[j].Position.Position.z + 205) + matrix_content[13]) / Camera * (dInfo.height / 2);

            // float X = r_x - (r_y - r_w) / 4;
            // float Y = r_y;
            // float W = (r_y - r_w) / 2;

            // int x = players[i].Position.Position.x - (players[i].Position.Position.y - players[i].Position.Position.z) / 4;
            // int y = players[i].Position.Position.y;
            // int w = (players[i].Position.Position.y - players[i].Position.Position.z) / 2;
            // ImGui::GetForegroundDrawList()->AddRect(ImVec2(X, Y - W), ImVec2(X + W, Y + W), RED, 0, 0, 2.0f);
            // ImGui::GetForegroundDrawList()->AddRect(ImVec2(x, y - w), ImVec2(x + w, y + w), RED, 0, 0, 2.0f);

            Mesh = driver->read<uintptr_t>(Object + 0x5C8);
            Bone = driver->read<uintptr_t>(Mesh + 0x730) + 0x30;
            if (Mesh < 0xFFFF || Bone < 0xFFFF)
                continue;

            int BoneCount = driver->read<uintptr_t>(Mesh + 0x738);
            int p = (BoneCount == 68) ? 33 : 32;
            int o = (BoneCount == 68) ? 34 : 33;
            int a = (BoneCount == 68) ? 13 : 63;
            int b = (BoneCount == 68) ? 35 : 62;
            int c = (BoneCount == 68) ? 55 : 52;
            int d = (BoneCount == 68) ? 59 : 56;
            int e = (BoneCount == 68) ? 56 : 53;
            int f = (BoneCount == 68) ? 60 : 57;
            int g = (BoneCount == 68) ? 57 : 54;
            int h = (BoneCount == 68) ? 61 : 58;

            WorldToScreen(&players[j].Position.Screen_Position, &Camera, &players[j].Width, players[j].Position.Position, matrix_content, dInfo.width, dInfo.height);

            struct FMatrix C2W_Matrix = TransformToMatrix(getBone(Mesh + 0x1B0, driver));

            players[j].Head.Position = MarixToVector(
                MatrixMulti(
                    TransformToMatrix(getBone(Bone + 5 * 48, driver)), C2W_Matrix
                )
            );
            players[j].Head.Position.z += 7;
            players[j].Head.Screen_Position = WorldToScreen(
                players[j].Head.Position,
                matrix_content,
                dInfo.width / 2,
                dInfo.height / 2
            );

            players[j].Chest.Position = MarixToVector(
                MatrixMulti(
                    TransformToMatrix(getBone(Bone + 4 * 48, driver)), C2W_Matrix
                )
            );
            players[j].Chest.Screen_Position = WorldToScreen(
                players[j].Chest.Position,
                matrix_content,
                dInfo.width / 2,
                dInfo.height / 2
            );

            players[j].Pelvis.Position = MarixToVector(
                MatrixMulti(
                    TransformToMatrix(getBone(Bone + 0 * 48, driver)), C2W_Matrix
                )
            );
            players[j].Pelvis.Screen_Position = WorldToScreen(
                players[j].Pelvis.Position,
                matrix_content,
                dInfo.width / 2,
                dInfo.height / 2
            );

            players[j].lSh.Position = MarixToVector(
                MatrixMulti(
                    TransformToMatrix(getBone(Bone + 11 * 48, driver)), C2W_Matrix
                )
            );
            players[j].lSh.Screen_Position = WorldToScreen(
                players[j].lSh.Position,
                matrix_content,
                dInfo.width / 2,
                dInfo.height / 2
            );

            players[j].rSh.Position = MarixToVector(
                MatrixMulti(
                    TransformToMatrix(getBone(Bone + p * 48, driver)), C2W_Matrix
                )
            );
            players[j].rSh.Screen_Position = WorldToScreen(
                players[j].rSh.Position,
                matrix_content,
                dInfo.width / 2,
                dInfo.height / 2
            );

            players[j].lElbow.Position = MarixToVector(
                MatrixMulti(
                    TransformToMatrix(getBone(Bone + 12 * 48, driver)), C2W_Matrix
                )
            );
            players[j].lElbow.Screen_Position = WorldToScreen(
                players[j].lElbow.Position,
                matrix_content,
                dInfo.width / 2,
                dInfo.height / 2
            );

            players[j].rElbow.Position = MarixToVector(
                MatrixMulti(
                    TransformToMatrix(getBone(Bone + o * 48, driver)), C2W_Matrix
                )
            );
            players[j].rElbow.Screen_Position = WorldToScreen(
                players[j].rElbow.Position,
                matrix_content,
                dInfo.width / 2,
                dInfo.height / 2
            );

            players[j].lWrist.Position = MarixToVector(
                MatrixMulti(
                    TransformToMatrix(getBone(Bone + a * 48, driver)), C2W_Matrix
                )
            );
            players[j].lWrist.Screen_Position = WorldToScreen(
                players[j].lWrist.Position,
                matrix_content,
                dInfo.width / 2,
                dInfo.height / 2
            );

            players[j].rWrist.Position = MarixToVector(
                MatrixMulti(
                    TransformToMatrix(getBone(Bone + b * 48, driver)), C2W_Matrix
                )
            );
            players[j].rWrist.Screen_Position = WorldToScreen(
                players[j].rWrist.Position,
                matrix_content,
                dInfo.width / 2,
                dInfo.height / 2
            );

            players[j].lLeg.Position = MarixToVector(
                MatrixMulti(
                    TransformToMatrix(getBone(Bone + c * 48, driver)), C2W_Matrix
                )
            );
            players[j].lLeg.Screen_Position = WorldToScreen(
                players[j].lLeg.Position,
                matrix_content,
                dInfo.width / 2,
                dInfo.height / 2
            );

            players[j].rLeg.Position = MarixToVector(
                MatrixMulti(
                    TransformToMatrix(getBone(Bone + d * 48, driver)), C2W_Matrix
                )
            );
            players[j].rLeg.Screen_Position = WorldToScreen(
                players[j].rLeg.Position,
                matrix_content,
                dInfo.width / 2,
                dInfo.height / 2
            );

            players[j].lKnee.Position = MarixToVector(
                MatrixMulti(
                    TransformToMatrix(getBone(Bone + e * 48, driver)), C2W_Matrix
                )
            );
            players[j].lKnee.Screen_Position = WorldToScreen(
                players[j].lKnee.Position,
                matrix_content,
                dInfo.width / 2,
                dInfo.height / 2
            );

            players[j].rKnee.Position = MarixToVector(
                MatrixMulti(
                    TransformToMatrix(getBone(Bone + f * 48, driver)), C2W_Matrix
                )
            );
            players[j].rKnee.Screen_Position = WorldToScreen(
                players[j].rKnee.Position,
                matrix_content,
                dInfo.width / 2,
                dInfo.height / 2
            );

            players[j].lAnkle.Position = MarixToVector(
                MatrixMulti(
                    TransformToMatrix(getBone(Bone + g * 48, driver)), C2W_Matrix
                )
            );
            players[j].lAnkle.Screen_Position = WorldToScreen(
                players[j].lAnkle.Position,
                matrix_content,
                dInfo.width / 2,
                dInfo.height / 2
            );

            players[j].rAnkle.Position = MarixToVector(
                MatrixMulti(
                    TransformToMatrix(getBone(Bone + h * 48, driver)), C2W_Matrix
                )
            );
            players[j].rAnkle.Screen_Position = WorldToScreen(
                players[j].rAnkle.Position,
                matrix_content,
                dInfo.width / 2,
                dInfo.height / 2
            );

            if (players[j].Head.Screen_Position.x > 0 &&
                players[j].Head.Screen_Position.y > 0 &&
                players[j].Width > 0 &&
                players[j].Head.Screen_Position.x < dInfo.width &&
                players[j].Head.Screen_Position.y < dInfo.height)
            {
                // ImGui::GetForegroundDrawList()->AddCircle(
                //     players[j].Head.Screen_Position,
                //     160 / sqrt(pow(players[j].Head.Position.x - My_pos.x, 2) + pow(players[j].Head.Position.y - My_pos.y, 2) + pow(players[j].Head.Position.z - My_pos.z, 2)),
                //     RED,
                //     6,
                //     4
                // );
                ImGui::GetForegroundDrawList()->AddLine(
                    ImVec2((float) dInfo.width / 2, 100),
                    players[j].Head.Screen_Position,
                    RED,
                    line_width
                );
                ImGui::GetForegroundDrawList()->AddLine(
                    players[j].Chest.Screen_Position,
                    players[j].Pelvis.Screen_Position,
                    GREEN,
                    line_width
                );
                ImGui::GetForegroundDrawList()->AddLine(
                    players[j].Chest.Screen_Position,
                    players[j].lSh.Screen_Position,
                    GREEN,
                    line_width
                );
                ImGui::GetForegroundDrawList()->AddLine(
                    players[j].lSh.Screen_Position,
                    players[j].lElbow.Screen_Position,
                    GREEN,
                    line_width
                );
                ImGui::GetForegroundDrawList()->AddLine(
                    players[j].lElbow.Screen_Position,
                    players[j].lWrist.Screen_Position,
                    GREEN,
                    line_width
                );

                ImGui::GetForegroundDrawList()->AddLine(
                    players[j].Chest.Screen_Position,
                    players[j].rSh.Screen_Position,
                    GREEN,
                    line_width
                );
                ImGui::GetForegroundDrawList()->AddLine(
                    players[j].rSh.Screen_Position,
                    players[j].rElbow.Screen_Position,
                    GREEN,
                    line_width
                );
                ImGui::GetForegroundDrawList()->AddLine(
                    players[j].rElbow.Screen_Position,
                    players[j].rWrist.Screen_Position,
                    GREEN,
                    line_width
                );
                ImGui::GetForegroundDrawList()->AddLine(
                    players[j].Pelvis.Screen_Position,
                    players[j].lLeg.Screen_Position,
                    GREEN,
                    line_width
                );
                ImGui::GetForegroundDrawList()->AddLine(
                    players[j].lLeg.Screen_Position,
                    players[j].lKnee.Screen_Position,
                    GREEN,
                    line_width
                );
                ImGui::GetForegroundDrawList()->AddLine(
                    players[j].lKnee.Screen_Position,
                    players[j].lAnkle.Screen_Position,
                    GREEN,
                    line_width
                );
                ImGui::GetForegroundDrawList()->AddLine(
                    players[j].Pelvis.Screen_Position,
                    players[j].rLeg.Screen_Position,
                    GREEN,
                    line_width
                );
                ImGui::GetForegroundDrawList()->AddLine(
                    players[j].rLeg.Screen_Position,
                    players[j].rKnee.Screen_Position,
                    GREEN,
                    line_width
                );
                ImGui::GetForegroundDrawList()->AddLine(
                    players[j].rKnee.Screen_Position,
                    players[j].rAnkle.Screen_Position,
                    GREEN,
                    line_width
                );
            }
            ImGui::Text("x: %.2f\ty: %.2f\tz: %.2f\tHP: %.2f\n", players[j].Position.Position.x, players[j].Position.Position.y, players[j].Position.Position.z, players[j].Health);
            j++;
        }

end:
        _shutdown = ImGui::Button("close");
        ImGui::End();

        ImGui::Render();
        glViewport(0, 0, (int) w_h, (int) w_h);
        glClearColor(0,0,0,0/*clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w*/);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        eglSwapBuffers(eDisplay, eSurface);
    }

    puts("RELEASE WINDOW!");
    sleep(1);
    delete pth;
    delete d_info;
    delete driver;
    delete game_data;
    sleep(2);
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

struct FMatrix TransformToMatrix(const FTransform transform)
{
    FMatrix matrix;
    matrix.M[3][0] = transform.Translation.x;
    matrix.M[3][1] = transform.Translation.y;
    matrix.M[3][2] = transform.Translation.z;
    float x2 = transform.Rotation.x + transform.Rotation.x;
    float y2 = transform.Rotation.y + transform.Rotation.y;
    float z2 = transform.Rotation.z + transform.Rotation.z;
    float xx2 = transform.Rotation.x * x2;
    float yy2 = transform.Rotation.y * y2;
    float zz2 = transform.Rotation.z * z2;
    matrix.M[0][0] = (1 - (yy2 + zz2)) * transform.Scale3D.x;
    matrix.M[1][1] = (1 - (xx2 + zz2)) * transform.Scale3D.y;
    matrix.M[2][2] = (1 - (xx2 + yy2)) * transform.Scale3D.z;
    float yz2 = transform.Rotation.y * z2;
    float wx2 = transform.Rotation.w * x2;
    matrix.M[2][1] = (yz2 - wx2) * transform.Scale3D.z;
    matrix.M[1][2] = (yz2 + wx2) * transform.Scale3D.y;
    float xy2 = transform.Rotation.x * y2;
    float wz2 = transform.Rotation.w * z2;
    matrix.M[1][0] = (xy2 - wz2) * transform.Scale3D.y;
    matrix.M[0][1] = (xy2 + wz2) * transform.Scale3D.x;
    float xz2 = transform.Rotation.x * z2;
    float wy2 = transform.Rotation.w * y2;
    matrix.M[2][0] = (xz2 + wy2) * transform.Scale3D.z;
    matrix.M[0][2] = (xz2 - wy2) * transform.Scale3D.x;
    matrix.M[0][3] = 0;
    matrix.M[1][3] = 0;
    matrix.M[2][3] = 0;
    matrix.M[3][3] = 1;
    return matrix;
}

inline Vec3 MarixToVector(const FMatrix matrix)
{
    return Vec3(matrix.M[3][0], matrix.M[3][1], matrix.M[3][2]);
}

FMatrix MatrixMulti(const FMatrix m1, const FMatrix m2)
{
    FMatrix matrix = FMatrix();
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                matrix.M[i][j] += m1.M[i][k] * m2.M[k][j];
            }
        }
    }
    return matrix;
}

void WorldToScreen(Vec2 *bscreen, float *camea, float *w, const Vec3 obj, const float *matrix, int Width, int Height)
{
    float camear = matrix[3] * obj.x + matrix[7] * obj.y + matrix[11] * obj.z + matrix[15];
    *camea = camear;
    bscreen->x = Width + (matrix[0] * obj.x + matrix[4] * obj.y + matrix[8] * obj.z + matrix[12]) / camear * Width;
    bscreen->y = Height - (matrix[1] * obj.x + matrix[5] * obj.y + matrix[9] * obj.z + matrix[13]) / camear * Height;
    float bscreenZ = Height - (matrix[1] * obj.x + matrix[5] * obj.y + matrix[9] * (obj.z + 165) + matrix[13]) / camear * Height;
    float bscreenz = bscreen->y - bscreenZ;
    *w = (bscreen->y - bscreenZ) / 2;
}

Vec2 WorldToScreen(const Vec3 obj, const float *matrix, const int Width, const int Height)
{
    Vec2 bscreen;
    float camear = matrix[3] * obj.x + matrix[7] * obj.y + matrix[11] * obj.z + matrix[15];
    bscreen.x = Width + (matrix[0] * obj.x + matrix[4] * obj.y + matrix[8] * obj.z + matrix[12]) / camear * Width;
    bscreen.y = Height - (matrix[1] * obj.x + matrix[5] * obj.y + matrix[9] * obj.z + matrix[13]) / camear * Height;
    return bscreen;
}

void signal_terminate(int sign)
{
    _shutdown = true;
    exit(EXIT_SUCCESS);
}

struct FTransform getBone(uintptr_t addr, c_driver *driver)
{
    FTransform transform;
    driver->read(addr, reinterpret_cast<void*>(&transform), 4 * 11);
    return transform;
}

