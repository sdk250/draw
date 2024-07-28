#include "touch.hpp"

Touch::Touch(void)
{
    touch_device = new char [NAME_SIZE];

    if ((ndev = scandir("/dev/input", &name_list, this->get_input_device, alphasort)) <= 0)
        perror("Not found input device!");
    for(int i = 0, fd = -1; i++ < ndev;)
    {
        unsigned long keybit[NBITS(KEY_CNT)];
        snprintf(touch_device, NAME_SIZE, "%s/%s", "/dev/input", name_list[i]->d_name);
        if((fd = open(touch_device, O_RDONLY)) < 0)
        {
            free(name_list[i]);
            continue;
        }
        memset(keybit, 0, sizeof(keybit));
        ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keybit)), keybit);
        close(fd);
        free(name_list[i]);
        if (CBIT(keybit, BTN_TOUCH) || CBIT(keybit, BTN_TOOL_FINGER))
            break;
    }

    if ((fd = open(touch_device, O_RDONLY | O_SYNC | O_NONBLOCK)) < 0)
        perror("Open input device error!");
}

Touch::~Touch(void)
{
    delete [] touch_device;
    if (fd > 0)
        close(fd);
    delete touch_thread;
    puts("Release touch event done!");
}

int Touch::get_input_device(const struct dirent *dir)
{
    return !strncmp("event", dir->d_name, 5);
}

void Touch::start(ImGuiIO &io)
{
    touch_thread = new std::thread {[&] {
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
                switch (display_info.theta / 90)
                {
                    case 0:
                        ImGui::GetIO().MousePos = ImVec2(x, y);
                        continue;
                    case 1:
                        ImGui::GetIO().MousePos = ImVec2(y , display_info.height - x);
                        continue;
                    case 3:
                        ImGui::GetIO().MousePos = ImVec2(display_info.width - y, x);
                    default:
                        break;
                }
            }
        }
    }};
    touch_thread->detach();
}
