#include "chatter.h"
#include "argparse.h"

APP_CONFIG AppConfig;
LOG_CONFIG LogConfig;

VOID
PrintLogTimeStamp(
    VOID
    )
{
    SYSTEMTIME Time;
    GetLocalTime(&Time);
    printf(
        "[%02d:%02d:%02d.%03d] ",
        Time.wHour,
        Time.wMinute,
        Time.wSecond,
        Time.wMilliseconds);
}

VOID
RegisterDevices(
    _In_count_(DeviceCount) PRAWINPUTDEVICELIST DeviceList,
    _In_ UINT DeviceCount
    )
{
    UINT Count;
    PRAWINPUTDEVICELIST Device;
    LPWSTR DeviceName = NULL;
    RID_DEVICE_INFO DeviceInfo;
    BOOLEAN Register;

    for (Count = 0; Count < DeviceCount; Count += 1) {
        Device = &DeviceList[Count];
        if (!GetDeviceInfo(Device, &DeviceInfo)) {
            continue;
        }

        DeviceName = GetDeviceInstancePath(Device);
        if (DeviceName == NULL) {
            continue;
        }

        Register = TRUE;
        if (!AppConfig.UseVendorSpecificHidDevices &&
            Device->dwType == RIM_TYPEHID &&
            (DeviceInfo.hid.usUsagePage >> 8) == 0xff) {

            Register = FALSE;

            if (LogConfig.VerboseError) {
                printf(
                    "Skipping device with vendor-specific usage page, Device %S, usage page 0x%x\n",
                    DeviceName,
                    DeviceInfo.hid.usUsagePage);
            }
        }

        if (Register) {
            RegisterDevice(Device->hDevice, DeviceName, &DeviceInfo);
        }

        free(DeviceName);
        DeviceName = NULL;
    }
}

VOID
ProcessWmInput (
    _In_ PRAWINPUT Data
    )

{
    PREGISTERED_DEVICE Device;
    BOOLEAN Log;

    Device = FindRegisteredDevice(Data->header.hDevice);
    if (Device == NULL) {
        if (LogConfig.VerboseError) {
            printf(
                "Received input from unregistered device 0x%x\n",
                (DWORD)Data->header.hDevice);
        }
        return;
    }

    switch (Data->header.dwType) {
        case RIM_TYPEMOUSE:
            Log = CheckMouseEvent(&Data->data.mouse);
            break;
        case RIM_TYPEKEYBOARD:
            Log = CheckKeyboardEvent(&Data->data.keyboard);
            break;
        case RIM_TYPEHID:
            Log = CheckHidEvent(&Data->data.hid);
            break;
        default:
            Log = FALSE;
            break;
    }

    if (!Log) {
        return;
    }

    if (LogConfig.LogEvents) {
        PrintLogTimeStamp();
        printf("[Device Type: ");
        if (Data->header.dwType <= RIM_TYPEHID) {
            printf("%S", DeviceTypes[Data->header.dwType]);
        } else {
            printf("Unknown type %d", Data->header.dwType);
        }

        printf(", Device ID: 0x%x]\n", (DWORD)Data->header.hDevice);
    }

    switch (Data->header.dwType) {
        case RIM_TYPEMOUSE:
            ProcessMouseEvent(Device, &Data->data.mouse);
            break;
        case RIM_TYPEKEYBOARD:
            ProcessKeyboardEvent(Device, &Data->data.keyboard);
            break;
        case RIM_TYPEHID:
            ProcessHidEvent(Device, &Data->data.hid);
            break;
        default:
            break;
    }

    if (LogConfig.LogEvents) {
        printf("\n");
    }
}

DECLSPEC_NORETURN
VOID
FailFast (
    _In_ UINT ExitCode
    )

{
    printf("Exiting with error 0x%x", ExitCode);
    ExitProcess(ExitCode);
}

static const char *const usages[] = {
    "Use one of these modes with other optional flags:",
    "chatter.exe -c  --  chattering test mode",
    "chatter.exe -l  --  to see the list of HID devices",
    "chatter.exe -m  --  to monitor all HID input events",
    NULL,
};

int
main(
    int argc,
    char* argv[]
    )
{
    INT ArgChatter = FALSE;
    INT ArgCursor = FALSE;
    INT ArgList = FALSE;
    INT ArgMonitor = FALSE;
    INT ArgVerbose = FALSE;
    INT ArgVendorSpecific = FALSE;
    INT ArgDuration = 15;
    UINT DeviceCount;
    PRAWINPUTDEVICELIST DeviceList;
    struct argparse Argparse;
    struct argparse_option Options[] = {
        OPT_HELP(),
        OPT_BOOLEAN('c', "chatter", &ArgChatter, "(MODE) Chatter test mode", NULL, 0, 0),
        OPT_BOOLEAN('l', "list", &ArgList, "(MODE) List all raw input devices", NULL, 0, 0),
        OPT_BOOLEAN('m', "monitor", &ArgMonitor, "(MODE) Monitor all HID input events from device(s)", NULL, 0, 0),
        OPT_BOOLEAN('u', "cursor", &ArgCursor, "(Optional) Enable mouse movement tracing", NULL, 0, 0),
        OPT_BOOLEAN('v', "verbose", &ArgVerbose, "(Optional) Enable verbose prints", NULL, 0, 0),
        OPT_BOOLEAN('e', "vendor", &ArgVendorSpecific, "(Optional) Include vendor-specific devices", NULL, 0, 0),
        OPT_INTEGER('d', "duration", &ArgDuration, "(Optional) Set glitch duration in milliseconds, default 15ms", NULL, 0, 0),
        OPT_END(),
    };
    argparse_init(&Argparse, Options, usages, 0);
    argparse_describe(&Argparse, "\nchatter-cli - button tester for HID devices", NULL);
    if (argc < 2) {
        argparse_usage(&Argparse);
        printf("\n");
        system("pause");
        printf("\n");
        return -1;
    }
    argc = argparse_parse(&Argparse, argc, argv);
    if (!ArgChatter && !ArgList && !ArgMonitor) {
        argparse_usage(&Argparse);
        return -1;
    }

    AppConfig.UseVendorSpecificHidDevices = ArgVendorSpecific;
    AppConfig.GlitchDurationInMs = ArgDuration;

    LogConfig.LogChatter = ArgChatter;
    LogConfig.LogEvents = ArgMonitor;
    LogConfig.VerboseError = ArgVerbose;
    LogConfig.Mouse.Movement = ArgCursor;
    LogConfig.Mouse.ButtonClicks = TRUE;
    LogConfig.Hid.UsageText = ArgVerbose;

    InitTimerSupport();
    InitializeRegisteredList();

    DeviceList = GetDevices(&DeviceCount);
    if (DeviceList != NULL) {
        RegisterDevices(DeviceList, DeviceCount);
        free(DeviceList);
    }
    if (ArgList) {
        DumpRegisteredDevices();
    }
    if (ArgChatter || ArgMonitor) {
        printf("Monitoring raw input from %d device(s)\n", DeviceCount);
        printf("Press CTRL+C to exit any time.\n\n");
        CreateNewWindow();
    }

    DeinitializeRegisteredList();
    return 0;
}
