#pragma once

#include "chatter.h"

struct _BUTTON_CAP_STATE {
    // Length is 1 for NotRange, UsageMax-Min+1 for Range
    UINT Length;
    // equals Usage in NotRange, UsageMin for Range
    USAGE UsageMin;
    _Field_size_full_(Length) PBOOLEAN PreviousState;
    _Field_size_full_(Length) PBOOLEAN CurrentState;
    _Field_size_full_(Length) PCHAR *UsageText;
};

struct _DPAD_CAP_STATE {
    USHORT NthInputValueCap;
    USHORT NthUsage;
    DPAD_DIRECTION PreviousState;
    DPAD_DIRECTION CurrentState;
};

struct _REGISTERED_HID_DEVICE_INFO {
    WCHAR Manufacturer[128];
    WCHAR Product[128];
    WCHAR SerialNumber[128];
    HIDP_CAPS HidCaps;

    _Field_size_full_(HidCaps.NumberInputButtonCaps) PHIDP_BUTTON_CAPS ButtonCaps;
    _Field_size_full_(HidCaps.NumberInputButtonCaps) PBUTTON_CAP_STATE *ButtonCapStates;
    
    _Field_size_full_(HidCaps.NumberInputValueCaps) PHIDP_VALUE_CAPS ValueCaps;

    USHORT DpadCount;
    _Field_size_full_(DpadCount) DPAD_CAP_STATE DpadCaps;

    PHIDP_PREPARSED_DATA PreparsedData;
    list_t *TimerList;
};

VOID
DumpHid(
    _In_ PRID_DEVICE_INFO_HID Hid,
    _In_ PREGISTERED_HID_DEVICE_INFO HidInfo
    );

BOOLEAN
CheckHidEvent(
    _In_ PRAWHID Hid
    );

VOID
ProcessHidEvent (
    _In_ PREGISTERED_DEVICE Device,
    _In_ PRAWHID Hid
    );

PREGISTERED_HID_DEVICE_INFO
RegisterHidDevice(
    _In_ HANDLE Handle,
    _In_ LPWSTR InstancePath
    );

VOID
FreeRegisteredHidDevice(
    _In_ _Frees_ptr_ PREGISTERED_HID_DEVICE_INFO HidDeviceInfo
    );
