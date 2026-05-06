#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dwmapi.h>

void enableDarkMode(void* hwnd)
{
    BOOL dark = TRUE;
    DwmSetWindowAttribute((HWND)hwnd, 20, &dark, sizeof(dark));
}
