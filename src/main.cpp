#include <Geode/Geode.hpp>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace geode::prelude;

LONG_PTR oWindowProc;
bool newWindowProcSet = false;

LRESULT CALLBACK nWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    if(msg == WM_KILLFOCUS) {
        if (CCDirector* CCDirector = cocos2d::CCDirector::get()) {
            CCDirector->getKeyboardDispatcher()->updateModifierKeys(false, false, false, false);
        }
    }

    return CallWindowProc((WNDPROC)oWindowProc, hwnd, msg, wparam, lparam);
}

HWND getWindowHandle() {
    return WindowFromDC(wglGetCurrentDC());
}

$on_mod(Loaded) {
    HWND hwnd = getWindowHandle();
    if (hwnd && !oWindowProc) {
        oWindowProc = SetWindowLongPtrA(getWindowHandle(), GWLP_WNDPROC, (LONG_PTR)nWindowProc);
    }
};