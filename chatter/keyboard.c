#include "keyboard.h"

#define KB_ALIGN 24
#define KBE_ALIGN 12

VOID
DumpKeyboardEvent (
    _In_ PRAWKEYBOARD Keyboard
    );

VOID
DumpKeyboard(
    _In_ PRID_DEVICE_INFO_KEYBOARD Keyboard
    )
{

    printf("%-*s: 0x%x ", KB_ALIGN, "dwType", Keyboard->dwType);
    if (Keyboard->dwType == 0x4) {
        printf("ENHANCED_101_OR_102_COMPATIBLE");
    } else if (Keyboard->dwType == 0x7) {
        printf("JAPANESE");
    } else if (Keyboard->dwType == 0x8) {
        printf("KOREAN");
    } else if (Keyboard->dwType == 0x51) {
        printf("HID_KEYBOARD_OR_UNKNOWN_TYPE");
    }
    printf("\n");

    printf("%-*s: %d\n", KB_ALIGN, "dwSubType", Keyboard->dwSubType);
    printf("%-*s: %d\n", KB_ALIGN,
        "dwKeyboardMode", Keyboard->dwKeyboardMode);
    printf("%-*s: %d\n", KB_ALIGN,
        "dwNumberOfFunctionKeys", Keyboard->dwNumberOfFunctionKeys);
    printf("%-*s: %d\n", KB_ALIGN,
        "dwNumberOfIndicators", Keyboard->dwNumberOfIndicators);
    printf("%-*s: %d\n", KB_ALIGN,
        "dwNumberOfKeysTotal", Keyboard->dwNumberOfKeysTotal);
}

PREGISTERED_KEYBOARD_DEVICE_INFO
RegisterKeyboardDevice(
    _In_ HANDLE Handle,
    _In_ LPWSTR InstancePath
    )
{
    PREGISTERED_KEYBOARD_DEVICE_INFO Keyboard;

    Keyboard = malloc(sizeof(*Keyboard));
    if (Keyboard == NULL) {
        FailFast(E_OUTOFMEMORY);
    }
    ZeroMemory(Keyboard, sizeof(*Keyboard));
    Keyboard->TimerList = InitTimerList();
    if (Keyboard->TimerList == NULL) {
        FailFast(E_OUTOFMEMORY);
    }
    return Keyboard;
}

VOID
FreeRegisteredKeyboardDevice(
    _In_ _Frees_ptr_ PREGISTERED_KEYBOARD_DEVICE_INFO Keyboard
    )
{
    if (Keyboard->TimerList != NULL) {
        FreeTimerList(Keyboard->TimerList);
        Keyboard->TimerList = NULL;
    }
    free(Keyboard);
}

BOOLEAN
CheckKeyboardEvent (
    _In_ PRAWKEYBOARD Keyboard
    )
{
    return TRUE;
}

VOID
ProcessKeyboardEvent (
    _In_ PREGISTERED_DEVICE Device,
    _In_ PRAWKEYBOARD Keyboard
    )
{
    ULONG MakeCode;
    UINT TimeInMs;
    WCHAR KeyCode[64];

    if (LogConfig.LogEvents) {
        DumpKeyboardEvent(Keyboard);
    }

    MakeCode = (Keyboard->MakeCode << 16);
    if (Keyboard->Flags & RI_KEY_E0) {
        MakeCode |= (1 << 24);
    }

    if (!LogConfig.LogChatter) {
        return;
    }

    // chatter processing
    switch (Keyboard->Message){
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            StartTimer(Device->KeyboardDeviceInfo->TimerList, MakeCode, TRUE);
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            TimeInMs = StopTimerAndGetMsElapsed(
                Device->KeyboardDeviceInfo->TimerList, MakeCode);
            PrintLogTimeStamp();
            if (0 < GetKeyNameText((LONG)MakeCode, KeyCode, ARRAYSIZE(KeyCode))) {
                printf("Duration (%-*S): %*d ms", 16, KeyCode, 6, TimeInMs);
            } else {
                printf("Duration (MakeCode %d): %*d ms", MakeCode, 6, TimeInMs);
            }

            if (TimeInMs <= AppConfig.GlitchDurationInMs) {
                printf("\t***GLITCH!!***");
            }

            printf("\n");
            break;
    }
}

VOID
DumpKeyboardEvent (
    _In_ PRAWKEYBOARD Keyboard
    )

{
    ULONG MakeCode;
    WCHAR KeyCode[64];

    printf("%-*s: 0x%x ", KBE_ALIGN, "MakeCode", Keyboard->MakeCode);
    if (Keyboard->MakeCode == KEYBOARD_OVERRUN_MAKE_CODE) {
        printf("KEYBOARD_OVERRUN_MAKE_CODE ");
    }
    MakeCode = (Keyboard->MakeCode << 16);
    if (Keyboard->Flags & RI_KEY_E0) {
        MakeCode |= (1 << 24);
    }
    if (0 < GetKeyNameText((LONG)MakeCode, KeyCode, ARRAYSIZE(KeyCode))) {
        printf("%S ", KeyCode);
    }
    printf("\n");

    printf("%-*s: 0x%x ", KBE_ALIGN, "Flags", Keyboard->Flags);
    if ((Keyboard->Flags & RI_KEY_BREAK) == 0) { // RI_KEY_MAKE
        printf("RI_KEY_MAKE ");
    }
    if (Keyboard->Flags & RI_KEY_BREAK) {
        printf("RI_KEY_BREAK ");
    }
    if (Keyboard->Flags & RI_KEY_E0) {
        printf("RI_KEY_E0 ");
    }
    if (Keyboard->Flags & RI_KEY_E1) {
        printf("RI_KEY_E1 ");
    }
    printf("\n");

    printf("%-*s: 0x%x %S\n", KBE_ALIGN,
        "VKey", Keyboard->VKey, GetVKeyText(Keyboard->VKey));

    printf("%-*s: 0x%x ", KBE_ALIGN, "Message", Keyboard->Message);
    if (Keyboard->Message == WM_ACTIVATE) {
        printf("WM_ACTIVATE ");
    } else if (Keyboard->Message == WM_APPCOMMAND) {
        printf("WM_APPCOMMAND ");
    } else if (Keyboard->Message == WM_CHAR) {
        printf("WM_CHAR ");
    } else if (Keyboard->Message == WM_DEADCHAR) {
        printf("WM_DEADCHAR ");
    } else if (Keyboard->Message == WM_HOTKEY) {
        printf("WM_HOTKEY ");
    } else if (Keyboard->Message == WM_KEYDOWN) {
        printf("WM_KEYDOWN ");
    } else if (Keyboard->Message == WM_KEYUP) {
        printf("WM_KEYUP ");
    } else if (Keyboard->Message == WM_KILLFOCUS) {
        printf("WM_KILLFOCUS ");
    } else if (Keyboard->Message == WM_SETFOCUS) {
        printf("WM_SETFOCUS ");
    } else if (Keyboard->Message == WM_SYSDEADCHAR) {
        printf("WM_SYSDEADCHAR ");
    } else if (Keyboard->Message == WM_SYSKEYDOWN) {
        printf("WM_SYSKEYDOWN ");
    } else if (Keyboard->Message == WM_SYSKEYUP) {
        printf("WM_SYSKEYUP ");
    } else if (Keyboard->Message == WM_UNICHAR) {
        printf("WM_UNICHAR ");
    }
    printf("\n");

    printf("%-*s: 0x%x\n", KBE_ALIGN,
        "ExtraInfo", Keyboard->ExtraInformation);
}
