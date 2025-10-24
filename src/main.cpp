static HWINEVENTHOOK g_fgHook = nullptr;
static HWINEVENTHOOK g_minHook = nullptr;

static bool isGameTopLevel(HWND h) {
    if (!h) return false;
    HWND top = GetAncestor(h, GA_ROOT);
    DWORD pid = 0;
    GetWindowThreadProcessId(top, &pid);
    return pid == GetCurrentProcessId();
}

static void CALLBACK WinEvtProc(HWINEVENTHOOK, DWORD event, HWND hwnd, LONG, LONG, DWORD, DWORD) {
    switch (event) {
        case EVENT_SYSTEM_FOREGROUND: {
            if (!isGameTopLevel(hwnd)) {
                geode::queueInMainThread([] {
                    if (auto director = cocos2d::CCDirector::get()) {
                        director->getKeyboardDispatcher()->updateModifierKeys(false, false, false, false);
                    }
                });
            }
        } break;
        case EVENT_SYSTEM_MINIMIZESTART: {
            if (isGameTopLevel(hwnd)) {
                geode::queueInMainThread([] {
                    if (auto director = cocos2d::CCDirector::get()) {
                        director->getKeyboardDispatcher()->updateModifierKeys(false, false, false, false);
                    }
                });
            }
        } break;
    }
}

$on_mod(Loaded) {
    g_fgHook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,
        nullptr, WinEvtProc, 0, 0, WINEVENT_OUTOFCONTEXT
    );
    g_minHook = SetWinEventHook(
        EVENT_SYSTEM_MINIMIZESTART, EVENT_SYSTEM_MINIMIZESTART,
        nullptr, WinEvtProc, 0, 0, WINEVENT_OUTOFCONTEXT
    );
}

$on_mod(DataSaved) {
    if (g_fgHook) { UnhookWinEvent(g_fgHook); g_fgHook = nullptr; }
    if (g_minHook) { UnhookWinEvent(g_minHook); g_minHook = nullptr; }
}
