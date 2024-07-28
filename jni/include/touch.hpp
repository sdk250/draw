#pragma once

#ifndef __TOUCH_H__
#define __TOUCH_H__

#include <sys/ioctl.h>
#include <linux/input.h>
#include <fcntl.h>
#include <asm/fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <thread>

// #include "global_value.hpp"
#include "imgui.h"
#include "ANativeWindowCreator.hpp"

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) (( ((x) - 1) / BITS_PER_LONG) + 1)
#define CBIT(ARRAY, BIT) ( \
    ( \
        ARRAY[BIT / BITS_PER_LONG] >> BIT % BITS_PER_LONG \
    ) & 1 \
)
#define NAME_SIZE 64

extern struct android::ANativeWindowCreator::DisplayInfo display_info;
extern bool _shutdown;

class Touch
{
    public:
        Touch(void);
        ~Touch(void);
        void start(ImGuiIO &io);
        int get_touch_device(char * name);
        static int get_input_device(const struct dirent *dir);
        inline const char * get_device_name(void) {return touch_device; };
    private:
        struct input_event touch_event {};
        struct dirent **name_list {nullptr};
        char *touch_device {nullptr};
        int fd {-1}, ndev {-1};
        float x {0.0}, y {0.0}, z {0.0};
        std::thread *touch_thread {nullptr};
};

#endif // TOUCH_H