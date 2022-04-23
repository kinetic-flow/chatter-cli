#pragma once

#include "chatter.h"

struct _REGISTERED_MOUSE_DEVICE_INFO {
    list_t *TimerList;
};

PREGISTERED_MOUSE_DEVICE_INFO
RegisterMouseDevice(
    _In_ HANDLE Handle,
    _In_ LPWSTR InstancePath
    );

VOID
FreeRegisteredMouseDevice(
    _In_ _Frees_ptr_ PREGISTERED_MOUSE_DEVICE_INFO Mouse
    );

VOID
DumpMouse(
    _In_ PRID_DEVICE_INFO_MOUSE Mouse
    );

BOOLEAN
CheckMouseEvent (
    _In_ PRAWMOUSE Mouse
    );

VOID
ProcessMouseEvent (
    _In_ PREGISTERED_DEVICE Device,
    _In_ PRAWMOUSE Mouse
    );
