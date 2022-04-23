#include "devicelist.h"

PRAWINPUTDEVICELIST
GetDevices(
    _Out_ PUINT DeviceCountOut
    )
{

    UINT ActualDeviceCount = 0;
    UINT DeviceCount = 0;
    PRAWINPUTDEVICELIST DeviceList = NULL;

    *DeviceCountOut = 0;

    // Check how many devices exist
    ActualDeviceCount = GetRawInputDeviceList(
        NULL, &DeviceCount, sizeof(DeviceList[0]));
    if (ActualDeviceCount < 0) {
        printf("GetRawInputDeviceList failed, GLE = 0x%x\n", GetLastError());
        return NULL;
    }
    if (DeviceCount == 0) {
        printf("GetRawInputDeviceList returned no devices\n");
        return NULL;
    }

    DeviceList = malloc(sizeof(DeviceList[0]) * DeviceCount);
    while (TRUE) {
        ActualDeviceCount = GetRawInputDeviceList(
            DeviceList, &DeviceCount, sizeof(DeviceList[0]));

        // success
        if (0 <= ActualDeviceCount) {
            *DeviceCountOut = ActualDeviceCount;
            break;
        }

        // buffer too small - try again with a bigger buffer
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
            free(DeviceList);
            DeviceList = malloc(sizeof(DeviceList[0]) * DeviceCount);
            continue;
        }

        // some other error
        printf("GetRawInputDeviceList failed, GLE = 0x%x\n", GetLastError());
        free(DeviceList);
        DeviceList = NULL;
        break;
    }

    if (*DeviceCountOut == 0) {
        free(DeviceList);
        DeviceList = NULL;
    }

    printf("\n");
    return DeviceList;
}

BOOLEAN
GetDeviceInfo(
    _In_ PRAWINPUTDEVICELIST RawDevice,
    _Out_ PRID_DEVICE_INFO DeviceInfo
    )

{

    UINT BytesReturned;
    LPWSTR DeviceName = NULL;
    UINT RequiredSize = 0;

    ZeroMemory(DeviceInfo, sizeof(*DeviceInfo));
    DeviceInfo->cbSize = sizeof(*DeviceInfo);
    RequiredSize = sizeof(*DeviceInfo);
    BytesReturned = GetRawInputDeviceInfo(
        RawDevice->hDevice,
        RIDI_DEVICEINFO,
        DeviceInfo,
        &RequiredSize);

    if (BytesReturned == -1) {
        printf(
            "GetRawInputDeviceInfo failed, RequiredSize = 0x%x, GLE = 0x%x\n",
            RequiredSize,
            GetLastError());
        return FALSE;
    }

    return TRUE;
}

LPWSTR
GetDeviceInstancePath(
    _In_ PRAWINPUTDEVICELIST RawDevice
    )

{

    UINT BytesReturned;
    LPWSTR DeviceName = NULL;
    UINT RequiredSize = 0;

    BytesReturned = GetRawInputDeviceInfo(
        RawDevice->hDevice,
        RIDI_DEVICENAME,
        NULL,
        &RequiredSize);

    if (BytesReturned != 0 || RequiredSize == 0) {
        printf(
            "GetRawInputDeviceInfo failed, RequiredSize = 0x%x, GLE = 0x%x\n",
            RequiredSize,
            GetLastError());
        return NULL;
    }

    DeviceName = malloc(sizeof(WCHAR) * (RequiredSize + 1));
    BytesReturned = GetRawInputDeviceInfo(
        RawDevice->hDevice,
        RIDI_DEVICENAME,
        (PVOID)DeviceName,
        &RequiredSize);

    if (BytesReturned == -1 || RequiredSize == 0) {
        printf(
            "GetRawInputDeviceInfo failed, RequiredSize = 0x%x, GLE = 0x%x\n",
            RequiredSize,
            GetLastError());
        free(DeviceName);
        DeviceName = NULL;
        return NULL;
    }

    return DeviceName;
}
