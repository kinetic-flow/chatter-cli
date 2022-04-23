#include <stdio.h>
#include <malloc.h>
#include <wchar.h>
#include <Windows.h>

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
            printf("Device Count %d\n", DeviceCount);
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
    return DeviceList;
}

VOID
DumpDevices(
    _In_count_(DeviceCount) PRAWINPUTDEVICELIST DeviceList,
    _In_ UINT DeviceCount
    )

{

    UINT BytesReturned;
    UINT Count;
    LPWSTR DeviceName = NULL;
    UINT RequiredSize = 0;

    if (DeviceList == NULL || DeviceCount == 0) {
        printf("No device found!\n");
        return;
    }

    for (Count = 0; Count < DeviceCount; Count += 1) {
        BytesReturned = GetRawInputDeviceInfo(
            DeviceList[Count].hDevice,
            RIDI_DEVICENAME,
            NULL,
            &RequiredSize);

        if (BytesReturned != 0 || RequiredSize == 0) {
            printf(
                "GetRawInputDeviceInfo failed, RequiredSize = 0x%x, GLE = 0x%x\n",
                RequiredSize,
                GetLastError());
            continue;
        }

        DeviceName = malloc(sizeof(WCHAR) * (RequiredSize + 1));
        BytesReturned = GetRawInputDeviceInfo(
            DeviceList[Count].hDevice,
            RIDI_DEVICENAME,
            (PVOID)DeviceName,
            &RequiredSize);

        if (BytesReturned == 0 || RequiredSize == 0) {
            printf(
                "GetRawInputDeviceInfo failed, RequiredSize = 0x%x, GLE = 0x%x\n",
                RequiredSize,
                GetLastError());
            free(DeviceName);
            DeviceName = NULL;
            continue;
        }

        printf("Device instance path: %S\n", DeviceName);

        free(DeviceName);
        DeviceName = NULL;
    }
}

int main() {
    UINT DeviceCount;
    PRAWINPUTDEVICELIST DeviceList;

    DeviceList = GetDevices(&DeviceCount);
    DumpDevices(DeviceList, DeviceCount);
    if (DeviceList != NULL) {
        free(DeviceList);
    }
    return 0;
}
