#pragma once

#include "chatter.h"

PRAWINPUTDEVICELIST
GetDevices(
    _Out_ PUINT DeviceCountOut
    );

BOOLEAN
GetDeviceInfo(
    _In_ PRAWINPUTDEVICELIST RawDevice,
    _Out_ PRID_DEVICE_INFO DeviceInfo
    );

LPWSTR
GetDeviceInstancePath(
    _In_ PRAWINPUTDEVICELIST RawDevice
    );
