#pragma once

#ifndef __INIT_DRAW_H__
#define __INIT_DRAW_H__

#include <android/native_window.h>

#include "VulkanGraphics.h"
#include "imgui_impl_android.h"
#include "ANativeWindowCreator.hpp"

class Init_draw: public VulkanGraphics
{
    public:
        Init_draw(ANativeWindow *_window, uint32_t width, uint32_t height);
        ~Init_draw(void);
    private:
        ANativeWindow *window {nullptr};
};


#endif // INIT_DRAW_H