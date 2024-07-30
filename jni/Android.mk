LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := window
LOCAL_CPP_EXTENSION := .cpp

LOCAL_SRC_FILES := src/main.cpp \
	src/imgui/imgui.cpp \
	src/imgui/imgui_draw.cpp \
	src/imgui/imgui_impl_android.cpp \
	src/imgui/imgui_impl_vulkan.cpp \
	src/imgui/imgui_tables.cpp \
	src/imgui/imgui_widgets.cpp \
	src/init_draw.cpp \
	src/Vulkan/vulkan_wrapper.cpp \
	src/Vulkan/VulkanGraphics.cpp \
	src/touch.cpp \
	src/stb_image.cpp \
	src/esp.cpp

LOCAL_LDLIBS := -static-libstdc++ \
	-landroid \
	-llog

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/imgui \
	$(LOCAL_PATH)/include/Vulkan \
	$(LOCAL_PATH)/include

LOCAL_CPPFLAGS := -O3 \
	-Wall \
	-ferror-limit=5

LOCAL_CFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR
LOCAL_CFLAGS += -DIMGUI_IMPL_VULKAN_NO_PROTOTYPES
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR
LOCAL_CPPFLAGS += -DIMGUI_IMPL_VULKAN_NO_PROTOTYPES

include $(BUILD_EXECUTABLE)

