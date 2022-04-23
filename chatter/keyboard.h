#pragma once

#include "chatter.h"

struct _REGISTERED_KEYBOARD_DEVICE_INFO {
    list_t *TimerList;
};

VOID
DumpKeyboard(
    _In_ PRID_DEVICE_INFO_KEYBOARD Keyboard
    );

PREGISTERED_KEYBOARD_DEVICE_INFO
RegisterKeyboardDevice(
    _In_ HANDLE Handle,
    _In_ LPWSTR InstancePath
    );

VOID
FreeRegisteredKeyboardDevice(
    _In_ _Frees_ptr_ PREGISTERED_KEYBOARD_DEVICE_INFO Keyboard
    );

BOOLEAN
CheckKeyboardEvent (
    _In_ PRAWKEYBOARD Keyboard
    );

VOID
ProcessKeyboardEvent (
    _In_ PREGISTERED_DEVICE Device,
    _In_ PRAWKEYBOARD Keyboard
    );
