#include "mouse.h"

VOID
DumpMouseEvent (
    _In_ PRAWMOUSE Mouse
    );

PREGISTERED_MOUSE_DEVICE_INFO
RegisterMouseDevice(
    _In_ HANDLE Handle,
    _In_ LPWSTR InstancePath
    )

{
    PREGISTERED_MOUSE_DEVICE_INFO MouseInfo = NULL;

    MouseInfo = malloc(sizeof(*MouseInfo));
    if (MouseInfo == NULL) {
        FailFast(E_OUTOFMEMORY);
    }
    ZeroMemory(MouseInfo, sizeof(*MouseInfo));
    MouseInfo->TimerList = InitTimerList();
    if (MouseInfo->TimerList == NULL) {
        FailFast(E_OUTOFMEMORY);
    }
    return MouseInfo;
}

VOID
FreeRegisteredMouseDevice(
    _In_ _Frees_ptr_ PREGISTERED_MOUSE_DEVICE_INFO Mouse
    )

{

    if (Mouse->TimerList != NULL) {
        FreeTimerList(Mouse->TimerList);
        Mouse->TimerList = NULL;
    }
    free(Mouse);
}

VOID
DumpMouse(
    _In_ PRID_DEVICE_INFO_MOUSE Mouse
    )
{
    printf("dwId:\t\t\t0x%x ", Mouse->dwId);
    if (Mouse->dwId & MOUSE_HID_HARDWARE) {
        printf("MOUSE_HID_HARDWARE ");
    }
    if (Mouse->dwId & WHEELMOUSE_HID_HARDWARE) {
        printf("WHEELMOUSE_HID_HARDWARE ");
    }
    if (Mouse->dwId & HORIZONTAL_WHEEL_PRESENT) {
        printf("HORIZONTAL_WHEEL_PRESENT ");
    }
    printf("\n");
    printf("dwNumberOfButtons:\t%d\n", Mouse->dwNumberOfButtons);
    printf("dwSampleRate:\t\t%d\n", Mouse->dwSampleRate);
    printf("fHasHorizontalWheel:\t%d\n", (DWORD)Mouse->fHasHorizontalWheel);
}

BOOLEAN
CheckMouseEvent (
    _In_ PRAWMOUSE Mouse
    )
{
    // ignore events that don't include mouse click events, unless
    // movement logging is enabled 
    if (!LogConfig.Mouse.Movement && Mouse->usButtonFlags == 0) {
        return FALSE;
    }

    return TRUE;
}

VOID
ProcessMouseUpEvent(
    _Inout_ list_t *TimerList,
    _In_ UINT MouseButton
    )
{
    UINT TimeInMs;
    
    TimeInMs = StopTimerAndGetMsElapsed(TimerList, MouseButton);
    PrintLogTimeStamp();
    printf("Duration (  Mouse button %d): %*d ms", MouseButton, 6, TimeInMs);
    if (TimeInMs <= AppConfig.GlitchDurationInMs) {
        printf("\t***GLITCH!!***");
    }

    printf("\n");
}

VOID
ProcessMouseEvent(
    _In_ PREGISTERED_DEVICE Device,
    _In_ PRAWMOUSE Mouse
    )
{
    list_t *List;

    if (LogConfig.LogEvents) {
        DumpMouseEvent(Mouse);
    }

    if (!LogConfig.LogChatter) {
        return;
    }

    List = Device->MouseDeviceInfo->TimerList;
    if (Mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
        StartTimer(List, 1, FALSE);
    }
    if (Mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
        StartTimer(List, 2, FALSE);
    }
    if (Mouse->usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
        StartTimer(List, 3, FALSE);
    }
    if (Mouse->usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) {
        StartTimer(List, 4, FALSE);
    }
    if (Mouse->usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) {
        StartTimer(List, 5, FALSE);
    }

    if (Mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
        ProcessMouseUpEvent(List, 1);
    }
    if (Mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {
        ProcessMouseUpEvent(List, 2);
    }
    if (Mouse->usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) {
        ProcessMouseUpEvent(List, 3);
    }
    if (Mouse->usButtonFlags & RI_MOUSE_BUTTON_4_UP) {
        ProcessMouseUpEvent(List, 4);
    }
    if (Mouse->usButtonFlags & RI_MOUSE_BUTTON_5_UP) {
        ProcessMouseUpEvent(List,5);
    }
}

VOID
DumpMouseEvent (
    _In_ PRAWMOUSE Mouse
    )
{
    if (LogConfig.Mouse.Verbose) {
        printf("usFlags:\t\t0x%x ", Mouse->usFlags);
        if (Mouse->usFlags & MOUSE_MOVE_RELATIVE) {
            printf("MOUSE_MOVE_RELATIVE ");
        }
        if (Mouse->usFlags & MOUSE_MOVE_ABSOLUTE) {
            printf("MOUSE_MOVE_ABSOLUTE ");
        }
        if (Mouse->usFlags & MOUSE_VIRTUAL_DESKTOP) {
            printf("MOUSE_VIRTUAL_DESKTOP ");
        }
        if (Mouse->usFlags & MOUSE_ATTRIBUTES_CHANGED) {
            printf("MOUSE_ATTRIBUTES_CHANGED ");
        }
        if (Mouse->usFlags & MOUSE_MOVE_NOCOALESCE) {
            printf("MOUSE_MOVE_NOCOALESCE ");
        }
        printf("\n");
    }

    printf("usButtonFlags:\t\t0x%x ", Mouse->usButtonFlags);
    if (Mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
        printf("RI_MOUSE_LEFT_BUTTON_DOWN ");
    }
    if (Mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
        printf("RI_MOUSE_LEFT_BUTTON_UP ");
    }
    if (Mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
        printf("RI_MOUSE_RIGHT_BUTTON_DOWN ");
    }
    if (Mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {
        printf("RI_MOUSE_RIGHT_BUTTON_UP ");
    }
    if (Mouse->usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
        printf("RI_MOUSE_MIDDLE_BUTTON_DOWN ");
    }
    if (Mouse->usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) {
        printf("RI_MOUSE_MIDDLE_BUTTON_UP ");
    }
    if (Mouse->usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) {
        printf("RI_MOUSE_BUTTON_4_DOWN ");
    }
    if (Mouse->usButtonFlags & RI_MOUSE_BUTTON_4_UP) {
        printf("RI_MOUSE_BUTTON_4_UP ");
    }
    if (Mouse->usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) {
        printf("RI_MOUSE_BUTTON_5_DOWN ");
    }
    if (Mouse->usButtonFlags & RI_MOUSE_BUTTON_5_UP) {
        printf("RI_MOUSE_BUTTON_5_UP ");
    }
    if (Mouse->usButtonFlags & RI_MOUSE_WHEEL) {
        printf("RI_MOUSE_WHEEL ");
    }
    if (Mouse->usButtonFlags & RI_MOUSE_HWHEEL) {
        printf("RI_MOUSE_HWHEEL ");
    }
    printf("\n");

    if (Mouse->usButtonFlags & RI_MOUSE_WHEEL ||
        Mouse->usButtonFlags & RI_MOUSE_HWHEEL) {
        printf("usButtonData:\t\t%d\n", (SHORT)Mouse->usButtonData);
    }

    if (LogConfig.Mouse.Verbose) {
        printf("ulRawButtons:\t\t0x%x\n", Mouse->ulRawButtons);
    }
    if (LogConfig.Mouse.Movement) {
        printf("lLastX, LastY:\t\t%d, %d\n", Mouse->lLastX, Mouse->lLastY);
    }
    if (LogConfig.Mouse.Verbose) {
        printf("ulExtraInformation:\t0x%x\n", Mouse->ulExtraInformation);
    }
}
