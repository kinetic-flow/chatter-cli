#pragma once

#include "chatter.h"

struct _REGISTERED_DEVICE {
    HANDLE Handle;
    PWSTR InstancePath;
    RID_DEVICE_INFO DeviceInfo;
    union {
        PVOID Generic;
        PREGISTERED_KEYBOARD_DEVICE_INFO KeyboardDeviceInfo;
        PREGISTERED_MOUSE_DEVICE_INFO MouseDeviceInfo;
        PREGISTERED_HID_DEVICE_INFO HidDeviceInfo;
    };
};

VOID
InitializeRegisteredList(
    VOID
    );

VOID
DeinitializeRegisteredList(
    VOID
    );

PREGISTERED_DEVICE
FindRegisteredDevice(
    _In_ HANDLE RawHandle
    );

VOID
RegisterDevice(
    _In_ HANDLE Handle,
    _In_ LPWSTR InstancePath,
    _In_ PRID_DEVICE_INFO DeviceInfo
    );

VOID
DumpRegisteredDevices(
    VOID
    );

extern LPWSTR DeviceTypes[];
