#include "registered.h"

list_t *RegisteredDeviceList;

LPWSTR DeviceTypes[] = {
    L"MOUSE",
    L"KEYBOARD",
    L"HID",
    L"Unknown"
};

VOID
InitializeRegisteredList(
    VOID
    )
{
    RegisteredDeviceList = list_new();
}

VOID
DeinitializeRegisteredList(
    VOID
    )
{
    list_node_t *Node;
    PREGISTERED_DEVICE RegisteredDevice;

    while (TRUE) {
        Node = list_rpop(RegisteredDeviceList);
        if (Node == NULL) {
            break;
        }
        if (Node->val != NULL) {
            RegisteredDevice = Node->val;
            switch (RegisteredDevice->DeviceInfo.dwType) {
                case RIM_TYPEMOUSE:
                    if (RegisteredDevice->MouseDeviceInfo != NULL) {
                        FreeRegisteredMouseDevice(RegisteredDevice->MouseDeviceInfo);
                        RegisteredDevice->MouseDeviceInfo = NULL;
                    }
                    break;
                case RIM_TYPEKEYBOARD:
                    if (RegisteredDevice->KeyboardDeviceInfo != NULL) {
                        FreeRegisteredKeyboardDevice(RegisteredDevice->KeyboardDeviceInfo);
                        RegisteredDevice->KeyboardDeviceInfo = NULL;
                    }
                    break;
                case RIM_TYPEHID:
                    if (RegisteredDevice->HidDeviceInfo != NULL) {
                        FreeRegisteredHidDevice(RegisteredDevice->HidDeviceInfo);
                        RegisteredDevice->HidDeviceInfo = NULL;
                    }
                    break;
                default:
                    break;
            }
            free(RegisteredDevice);
        }
        LIST_FREE(Node);
    }

    list_destroy(RegisteredDeviceList);
    RegisteredDeviceList = NULL;
}

PREGISTERED_DEVICE
FindRegisteredDevice(
    _In_ HANDLE RawHandle
    )
{
    PREGISTERED_DEVICE Device;
    list_node_t *node;
    list_iterator_t *it = list_iterator_new(RegisteredDeviceList, LIST_HEAD);

    Device = NULL;
    while (node = list_iterator_next(it)) {
        Device = node->val;
        if (Device->Handle == RawHandle) {
            break;
        }
    }

    list_iterator_destroy(it);
    if (node != NULL) {
        return Device;
    } else {
        return NULL;
    }
}

VOID
RegisterDevice(
    _In_ HANDLE Handle,
    _In_ LPWSTR InstancePath,
    _In_ PRID_DEVICE_INFO DeviceInfo
    )
{
    PREGISTERED_DEVICE RegisteredDevice;
    SIZE_T Length;

    RegisteredDevice = malloc(sizeof(*RegisteredDevice));
    if (RegisteredDevice == NULL) {
        FailFast(E_OUTOFMEMORY);
    }
    ZeroMemory(RegisteredDevice, sizeof(*RegisteredDevice));
    Length = wcslen(InstancePath) + 1;
    RegisteredDevice->InstancePath = malloc(sizeof(WCHAR) * Length);
    if (RegisteredDevice->InstancePath == NULL) {
        FailFast(E_OUTOFMEMORY);
    }
    RegisteredDevice->Handle = Handle;
    wcscpy_s(RegisteredDevice->InstancePath, Length, InstancePath);
    CopyMemory(&RegisteredDevice->DeviceInfo, DeviceInfo, sizeof(*DeviceInfo));

    switch (DeviceInfo->dwType) {
        case RIM_TYPEKEYBOARD:\
            RegisteredDevice->KeyboardDeviceInfo =
                RegisterKeyboardDevice(Handle, InstancePath);
            break;
        case RIM_TYPEMOUSE:
            RegisteredDevice->MouseDeviceInfo =
                RegisterMouseDevice(Handle, InstancePath);
            break;
        case RIM_TYPEHID:
            RegisteredDevice->HidDeviceInfo =
                RegisterHidDevice(Handle, InstancePath);
            break;
        default:
            break;
    }

    // finally, add to the list
    list_rpush(RegisteredDeviceList, list_node_new(RegisteredDevice));
}

VOID
DumpRegisteredDevices(
    VOID
    )

{

    UINT Count;
    LPWSTR DeviceName = NULL;
    PRID_DEVICE_INFO DeviceInfo;
    PREGISTERED_DEVICE Device;

    list_node_t *node;
    list_iterator_t *it = list_iterator_new(RegisteredDeviceList, LIST_HEAD);

    Count = 0;
    while (node = list_iterator_next(it)) {
        Device = node->val;
        DeviceInfo = &Device->DeviceInfo;

        // dump info
        printf("[Device #%d - ", Count);
        if (DeviceInfo->dwType <= RIM_TYPEHID) {
            printf("%S", DeviceTypes[DeviceInfo->dwType]);
        } else {
            printf("Unknown type %d", DeviceInfo->dwType);
        }

        printf(" (ID: %x)", (DWORD)Device->Handle);
        printf("]\n");

        // dump instance path
        printf("%S\n", Device->InstancePath);

        switch (DeviceInfo->dwType) {
            case RIM_TYPEKEYBOARD:
                DumpKeyboard(&DeviceInfo->keyboard);
                break;
            case RIM_TYPEMOUSE:
                DumpMouse(&DeviceInfo->mouse);
                break;
            case RIM_TYPEHID:
                DumpHid(&DeviceInfo->hid, Device->HidDeviceInfo);
                break;
            default:
                break;
        }
        printf("\n");
        Count += 1;
    }

    list_iterator_destroy(it);
}
