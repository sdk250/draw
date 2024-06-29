LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := window
LOCAL_SRC_FILES := window.cpp
LOCAL_CPP_EXTENSION := .cpp
LOCAL_LDLIBS := -static-libstdc++ \
	-landroid \
	-lEGL \
	-llog

LOCAL_CPPFLAGS := -O3 \
	-Wall \
	-ferror-limit=5

include $(BUILD_EXECUTABLE)

