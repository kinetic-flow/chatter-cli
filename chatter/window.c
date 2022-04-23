#include "window.h"

// Register the window class.
const wchar_t CLASS_NAME[]  = L"chatter-cli window";
HWND hwnd;

// pre-allocated for perf
PRAWINPUT RawInputData;
SIZE_T RawInputDataSize;

volatile LONG CtrlCDetected;

LRESULT
CALLBACK
WndProc(
    HWND hWnd,
    UINT msg,
    WPARAM wparam,
    LPARAM lParam
    )
{
    switch (msg) {
        case WM_CREATE: {
            LPCREATESTRUCT create_params = (LPCREATESTRUCT)lParam;
            SetWindowLongPtr(
                hWnd,
                GWLP_USERDATA,
                (LONG_PTR)(create_params->lpCreateParams));

            return 0;
        }
        case WM_INPUT: {
            UINT Size = RawInputDataSize;
            UINT BytesReturned;
            BytesReturned = GetRawInputData(
                (HRAWINPUT)lParam,
                RID_INPUT,
                RawInputData,
                &Size,
                sizeof(RAWINPUTHEADER));
            if (BytesReturned == -1) {
                printf("GetRawInputData failed, GLE = 0x%x\n", GetLastError());
                break;
            }
            ProcessWmInput(RawInputData);
            return 0;
        }
        default:
            break;
    }

    return DefWindowProc(hWnd, msg, wparam, lParam);
}

BOOL
WINAPI
ConsoleCtrlHandler(
    _In_ DWORD dwCtrlType
    )
{
    switch (dwCtrlType) {
        case CTRL_C_EVENT:
            printf("CTRL + C detected, exiting...\n");
            WriteNoFence(&CtrlCDetected, TRUE);
            if (hwnd != NULL) {
                PostMessage(hwnd, WM_CLOSE, 0, 0);
            }
            return TRUE;
        default:
            return FALSE;
    }
}

VOID
RegisterRawInput(
    VOID
    )
{
    RAWINPUTDEVICE device = {0};
    device.hwndTarget = hwnd;

    // keyboard
    device.dwFlags = RIDEV_INPUTSINK;
    device.usUsagePage = 1;
    device.usUsage = 0x06;
    if (!RegisterRawInputDevices(&device, 1, sizeof(device))) {
        printf("RegisterRawInputDevices failed, GLE = 0x%x\n", GetLastError());
    }

    // keypad
    device.dwFlags = RIDEV_INPUTSINK;
    device.usUsagePage = 1;
    device.usUsage = 0x07;
    if (!RegisterRawInputDevices(&device, 1, sizeof(device))) {
        printf("RegisterRawInputDevices failed, GLE = 0x%x\n", GetLastError());
    }

    // mouse
    device.dwFlags = RIDEV_INPUTSINK;
    device.usUsagePage = 1;
    device.usUsage = 0x02;
    if (!RegisterRawInputDevices(&device, 1, sizeof(device))) {
        printf("RegisterRawInputDevices failed, GLE = 0x%x\n", GetLastError());
    }

    // joystick
    device.dwFlags = RIDEV_INPUTSINK;
    device.usUsagePage = 1;
    device.usUsage = 0x04;
    if (!RegisterRawInputDevices(&device, 1, sizeof(device))) {
        printf("RegisterRawInputDevices failed, GLE = 0x%x\n", GetLastError());
    }

    // gamepad
    device.dwFlags = RIDEV_INPUTSINK;
    device.usUsagePage = 1;
    device.usUsage = 0x05;
    if (!RegisterRawInputDevices(&device, 1, sizeof(device))) {
        printf("RegisterRawInputDevices failed, GLE = 0x%x\n", GetLastError());
    }

    // digitizer
    device.dwFlags = RIDEV_PAGEONLY | RIDEV_INPUTSINK;
    device.usUsagePage = 0x0D;
    device.usUsage = 0x00;
    if (!RegisterRawInputDevices(&device, 1, sizeof(device))) {
        printf("RegisterRawInputDevices failed, GLE = 0x%x\n", GetLastError());
    }
}

DWORD
WINAPI
WindowThread(
    LPVOID lpParam
    ) 
{
    MSG msg;
    PWNDCLASSEX wc;

    wc = (PWNDCLASSEX)lpParam;

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

    // Create the window.
    hwnd = CreateWindowEx(
        WS_EX_NOACTIVATE,
        CLASS_NAME,
        L"chatter-cli window",
        0,
        0,
        0,
        0,
        0,
        NULL,
        NULL,
        wc->hInstance,
        NULL
        );

    if (hwnd == NULL) {
#if _DEBUG
        DebugBreak();
#endif

        printf("CreateWindowEx failed, GLE = 0x%x\n", GetLastError());
        return 0;
    }

    RawInputDataSize = sizeof(*RawInputData) + 0x10000;
    RawInputData = malloc(RawInputDataSize);
    RegisterRawInput();

    while (0 < GetMessage(&msg, hwnd, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DestroyWindow(hwnd);
    hwnd = NULL;
    return 0;
}

VOID
CreateNewWindow(
    VOID
    )

{

    HANDLE Thread;
    WNDCLASSEX wc = {0};

    SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;
    if (!RegisterClassEx(&wc)) {
        FailFast(GetLastError());
    }

    Thread = CreateThread(NULL, 0, WindowThread, &wc, 0, NULL);
    if (Thread == NULL) {
        FailFast(GetLastError());
    }
    WaitForSingleObject(Thread, INFINITE);

    UnregisterClass(CLASS_NAME, wc.hInstance);
}
