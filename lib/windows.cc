#include <napi.h>
#include <windows.h>
#include <string>

Napi::CallbackInfo *_info;

HHOOK _hook;

LRESULT CALLBACK HookCallback(int nCode, WPARAM wParam, LPARAM lParam) {
    auto cb = (*_info)[0].As<Napi::Function>();

    if (nCode >= 0) {
        MSLLHOOKSTRUCT *data = (MSLLHOOKSTRUCT *)lParam;

        auto x = Napi::Number::New((*_info).Env(), data->pt.x);
        auto y = Napi::Number::New((*_info).Env(), data->pt.y);

        auto name = "";
        auto which = "";

        if (wParam == WM_LBUTTONUP || wParam == WM_LBUTTONDOWN) {
            which = "mouse1";
        } else if (wParam == WM_RBUTTONUP || wParam == WM_RBUTTONDOWN) {
            which = "mouse2";
        }

        if (wParam == WM_LBUTTONUP || wParam == WM_RBUTTONUP) {
            name = "mouse-up";
        } else if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN) {
            name = "mouse-down";
        } else if (wParam == WM_MOUSEMOVE) {
            name = "mouse-move";
        }

        if (name != "") {
            cb.Call((*_info).Env().Global(),
                    {Napi::String::New((*_info).Env(), name), x, y,
                     Napi::String::New((*_info).Env(), which)});
        }
    }

    return CallNextHookEx(_hook, nCode, wParam, lParam);
}

Napi::Boolean createMouseHook(const Napi::CallbackInfo &info) {
    _info = &(const_cast<Napi::CallbackInfo &>(info));

    _hook = SetWindowsHookEx(WH_MOUSE_LL, HookCallback, NULL, 0);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return Napi::Boolean::New(info.Env(), true);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "createMouseHook"),
                Napi::Function::New(env, createMouseHook));

    return exports;
}

NODE_API_MODULE(addon, Init)