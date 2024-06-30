LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := window
LOCAL_SRC_FILES := src/window.cpp \
	src/imgui/imgui.cpp \
	src/imgui/imgui_demo.cpp \
	src/imgui/imgui_draw.cpp \
	src/imgui/imgui_impl_android.cpp \
	src/imgui/imgui_impl_opengl3.cpp \
	src/imgui/imgui_tables.cpp \
	src/imgui/imgui_widgets.cpp
LOCAL_CPP_EXTENSION := .cpp
LOCAL_LDLIBS := -static-libstdc++ \
	-landroid \
	-lEGL \
	-lGLESv3 \
	-llog
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/imgui \
	$(LOCAL_PATH)/include/imgui/backends \
	$(LOCAL_PATH)/include
LOCAL_CPPFLAGS := -O3 \
	-Wall \
	-ferror-limit=5

include $(BUILD_EXECUTABLE)

