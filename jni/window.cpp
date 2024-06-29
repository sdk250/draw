#include <iostream>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <assert.h>
#include "ANativeWindowCreator.hpp"

int main(void) 
{
	std::cout << __TIME__ << '\t' << __DATE__ << std::endl;

	ANativeWindow *native_window = nullptr;
	EGLDisplay eDisplay = EGL_NO_DISPLAY;
	EGLSurface eSurface = EGL_NO_SURFACE;
	EGLContext eContext = EGL_NO_CONTEXT;

	native_window = android::ANativeWindowCreator::Create(
		"sdk250",
		1080,
		2400
	);

	ANativeWindow_acquire(native_window);

	printf("Android native window: %8p\n", native_window);

	eDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	assert(eDisplay != EGL_NO_DISPLAY);
	assert(eglInitialize(eDisplay, nullptr, nullptr) == EGL_TRUE);

	puts("RELEASE WINDOW!");
	ANativeWindow_release(native_window);
	eglTerminate(eDisplay);
	native_window = nullptr;

	return 0;
}
