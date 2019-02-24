#include <node.h>
#include <windows.h>
#include <string>

void throwError(v8::Isolate *isolate, const char *error)
{
  isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, error)));
}

v8::Local<v8::Function> _cb;
v8::Isolate *_isolate;
HHOOK _hook;

LRESULT CALLBACK HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
  if (nCode >= 0)
  {
    MSLLHOOKSTRUCT *data = (MSLLHOOKSTRUCT *)lParam;

    auto x = v8::Number::New(_isolate, data->pt.x);
    auto y = v8::Number::New(_isolate, data->pt.y);

    auto name = "";
    auto which = "";

    if (wParam == WM_LBUTTONUP || wParam == WM_LBUTTONDOWN)
    {
      which = "mouse1";
    }
    else if (wParam == WM_RBUTTONUP || wParam == WM_RBUTTONDOWN)
    {
      which = "mouse2";
    }

    if (wParam == WM_LBUTTONUP || wParam == WM_RBUTTONUP)
    {
      name = "mouse-up";
    }
    else if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN)
    {
      name = "mouse-down";
    }

    if (name != "")
    {
      v8::Local<v8::Value> argv[4] = {v8::String::NewFromUtf8(_isolate, name), x, y, v8::String::NewFromUtf8(_isolate, which)};
      _cb->Call(v8::Null(_isolate), 4, argv);
    }
  }

  return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void createMouseHook(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();
  _isolate = isolate;

  _cb = v8::Local<v8::Function>::Cast(args[0]);

  _hook = SetWindowsHookEx(WH_MOUSE_LL, HookCallback, NULL, 0);

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0) > 0)
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

void Initialize(v8::Local<v8::Object> exports)
{
  NODE_SET_METHOD(exports, "createMouseHook", createMouseHook);
}

NODE_MODULE(windows_mouse_hooks, Initialize);