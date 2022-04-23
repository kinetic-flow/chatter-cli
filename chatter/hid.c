#include "hid.h"

PHIDP_PREPARSED_DATA
GetHidPreparsedData(
    _In_ HANDLE Handle
    );

VOID
DumpHidEvent(
    _In_ PREGISTERED_DEVICE Device,
    _In_ PRAWHID Hid
    );

VOID
DumpHid(
    _In_ PRID_DEVICE_INFO_HID Hid,
    _In_ PREGISTERED_HID_DEVICE_INFO HidInfo
    )
{
    PHIDP_BUTTON_CAPS ButtonCaps;
    PHIDP_VALUE_CAPS ValueCaps;
    PHIDP_CAPS Caps;
    UINT Count;
    PCHAR UsageText;
    USAGE Usage;
    BOOLEAN PrintAll;

    printf("dwVendorId:\t\t\t0x%x\n", Hid->dwVendorId);
    printf("dwProductId:\t\t\t0x%x\n", Hid->dwProductId);
    printf("dwVersionNumber:\t\t%d\n", Hid->dwVersionNumber);
    printf("usUsagePage:\t\t\t0x%x\n", Hid->usUsagePage);
    printf("usUsage:\t\t\t0x%x\n", Hid->usUsage);
    UsageText = getHidUsageText(Hid->usUsagePage, Hid->usUsage);
    if (UsageText != NULL) {
        printf("Usage Text:\t\t\t%s\n", UsageText);
        free(UsageText);
    }

    printf("---\n");

    printf("Manufacturer String:\t\t%S\n", HidInfo->Manufacturer);
    printf("Product String:\t\t\t%S\n", HidInfo->Product);
    printf("Serial Number String:\t\t%S\n", HidInfo->SerialNumber);

    printf("---\n");

    Caps = &HidInfo->HidCaps;
    printf("HID Usage:\t\t\t0x%x\n", Caps->Usage);
    printf("HID UsagePage:\t\t\t0x%x\n", Caps->UsagePage);
    printf("HID InputReportByteLength:\t%d\n", Caps->InputReportByteLength);
    printf("HID OutputReportByteLength:\t%d\n", Caps->OutputReportByteLength);
    printf("HID FeatureReportByteLength:\t%d\n", Caps->FeatureReportByteLength);
    printf("HID NumberLinkCollectionNodes:\t%d\n", Caps->NumberLinkCollectionNodes);
    printf("HID NumberInputButtonCaps:\t%d\n", Caps->NumberInputButtonCaps);
    printf("HID NumberInputValueCaps:\t%d\n", Caps->NumberInputValueCaps);
    printf("HID NumberInputDataIndices:\t%d\n", Caps->NumberInputDataIndices);
    printf("HID NumberOutputButtonCaps:\t%d\n", Caps->NumberOutputButtonCaps);
    printf("HID NumberOutputValueCaps:\t%d\n", Caps->NumberOutputValueCaps);
    printf("HID NumberOutputDataIndices:\t%d\n", Caps->NumberOutputDataIndices);
    printf("HID NumberFeatureButtonCaps:\t%d\n", Caps->NumberFeatureButtonCaps);
    printf("HID NumberFeatureValueCaps:\t%d\n", Caps->NumberFeatureValueCaps);
    printf("HID NumberFeatureDataIndices:\t%d\n", Caps->NumberFeatureDataIndices);

    for (Count = 0; Count < Caps->NumberInputButtonCaps; Count += 1) {
        printf("---\n");
        ButtonCaps = &HidInfo->ButtonCaps[Count];
        printf("ButtonCaps UsagePage:\t\t0x%x\n", ButtonCaps->UsagePage);
        printf("ButtonCaps ReportID:\t\t%d\n", ButtonCaps->ReportID);
        printf("ButtonCaps BitField:\t\t%d\n", ButtonCaps->BitField);
        printf("ButtonCaps LinkCollection:\t%d\n", ButtonCaps->LinkCollection);
        printf("ButtonCaps LinkUsage:\t\t%d\n", ButtonCaps->LinkUsage);
        printf("ButtonCaps LinkUsagePage:\t%d\n", ButtonCaps->LinkUsagePage);

        printf("ButtonCaps ...:\t\t\t");
        if (ButtonCaps->IsAlias) {
            printf("IsAlias ");
        }
        if (ButtonCaps->IsRange) {
            printf("IsRange ");
        }
        if (ButtonCaps->IsStringRange) {
            printf("IsStringRange ");
        }
        if (ButtonCaps->IsDesignatorRange) {
            printf("IsDesignatorRange ");
        }
        if (ButtonCaps->IsAbsolute) {
            printf("IsAbsolute ");
        }
        printf("\n");

        printf("ButtonCaps ReportCount:\t\t%d\n", ButtonCaps->ReportCount);
        if (ButtonCaps->IsRange) {
            printf("ButtonCaps Usage:\t\t[%d, %d]\n",
                ButtonCaps->Range.UsageMin, ButtonCaps->Range.UsageMax);

            if (LogConfig.Hid.UsageText) {
                PrintAll = TRUE;
            } else if ((ButtonCaps->Range.UsageMax - ButtonCaps->Range.UsageMin) < 63) {
                PrintAll = TRUE;
            } else {
                PrintAll = FALSE;
            }
            if (PrintAll) {
                printf("ButtonCaps Usage Text:\t\t");
                for (Usage = ButtonCaps->Range.UsageMin;
                    Usage <= ButtonCaps->Range.UsageMax;
                    Usage += 1) {
                    UsageText = getHidUsageText(ButtonCaps->UsagePage, Usage);
                    printf("%s", UsageText);
                    if (UsageText != NULL) {
                        free(UsageText);
                    }
                    if (Usage != ButtonCaps->Range.UsageMax) {
                        printf(", ");
                    }
                }
                printf("\n");
            } else {
                printf(
                    "ButtonCaps Usage Text:\t\t"
                    "Too many to print, use verbose mode to show all\n");
            }

            printf("ButtonCaps String:\t\t[%d, %d]\n",
                ButtonCaps->Range.StringMin, ButtonCaps->Range.StringMax);
            printf("ButtonCaps Designator:\t\t[%d, %d]\n",
                ButtonCaps->Range.DesignatorMin, ButtonCaps->Range.DesignatorMax);
            printf("ButtonCaps DataIndex:\t\t[%d, %d]\n",
                ButtonCaps->Range.DataIndexMin, ButtonCaps->Range.DataIndexMax);
        } else {
            printf(
                "ButtonCaps Usage:\t\t%d\n",
                ButtonCaps->NotRange.Usage);

            if (LogConfig.Hid.UsageText) {
                UsageText = getHidUsageText(ButtonCaps->UsagePage, ButtonCaps->NotRange.Usage);
                if (UsageText != NULL) {
                    printf("ButtonCaps Usage Text:\t\t%s\n", UsageText);
                    free(UsageText);
                }
            }

            printf(
                "ButtonCaps StringIndex:\t\t%d\n",
                ButtonCaps->NotRange.StringIndex);
            printf(
                "ButtonCaps DesignatorIndex:\t%d\n",
                ButtonCaps->NotRange.DesignatorIndex);
            printf(
                "ButtonCaps DataIndex:\t\t%d\n",
                ButtonCaps->NotRange.DataIndex);
        }
    }

    for (Count = 0; Count < Caps->NumberInputValueCaps; Count += 1) {
        printf("---\n");
        ValueCaps = &HidInfo->ValueCaps[Count];
        printf("ValueCaps UsagePage:\t\t0x%x\n", ValueCaps->UsagePage);
        printf("ValueCaps ReportID:\t\t%d\n", ValueCaps->ReportID);
        printf("ValueCaps BitField:\t\t%d\n", ValueCaps->BitField);
        printf("ValueCaps LinkCollection:\t%d\n", ValueCaps->LinkCollection);
        printf("ValueCaps LinkUsage:\t\t%d\n", ValueCaps->LinkUsage);
        printf("ValueCaps LinkUsagePage:\t%d\n", ValueCaps->LinkUsagePage);

        printf("ValueCaps ...:\t\t\t");
        if (ValueCaps->IsAlias) {
            printf("IsAlias ");
        }
        if (ValueCaps->IsRange) {
            printf("IsRange ");
        }
        if (ValueCaps->IsStringRange) {
            printf("IsStringRange ");
        }
        if (ValueCaps->IsDesignatorRange) {
            printf("IsDesignatorRange ");
        }
        if (ValueCaps->IsAbsolute) {
            printf("IsAbsolute ");
        }
        if (ValueCaps->HasNull) {
            printf("HasNull ");
        }
        printf("\n");

        printf("ValueCaps BitSize:\t\t%d\n", ValueCaps->BitSize);
        printf("ValueCaps ReportCount:\t\t%d\n", ValueCaps->ReportCount);
        printf("ValueCaps UnitsExp:\t\t0x%x\n", ValueCaps->UnitsExp);
        printf("ValueCaps Units:\t\t0x%x\n", ValueCaps->Units);
        printf("ValueCaps LogicalMin:\t\t0x%x\n", ValueCaps->LogicalMin);
        printf("ValueCaps LogicalMax:\t\t0x%x\n", ValueCaps->LogicalMax);
        printf("ValueCaps PhysicalMin:\t\t0x%x\n", ValueCaps->PhysicalMin);
        printf("ValueCaps PhysicalMax:\t\t0x%x\n", ValueCaps->PhysicalMax);

        if (ValueCaps->IsRange) {
            printf("ValueCaps Usage:\t\t[%d, %d]\n",
                ValueCaps->Range.UsageMin, ValueCaps->Range.UsageMax);
            if (LogConfig.Hid.UsageText) {
                printf("ValueCaps Usage Text:\t\t");
                for (Usage = ValueCaps->Range.UsageMin;
                    Usage <= ValueCaps->Range.UsageMax;
                    Usage += 1) {
                    UsageText = getHidUsageText(ValueCaps->UsagePage, Usage);
                    printf("%s", UsageText);
                    if (UsageText != NULL) {
                        free(UsageText);
                    }
                    if (Usage != ValueCaps->Range.UsageMax) {
                        printf(", ");
                    }
                }
                printf("\n");
            }

            printf("ValueCaps String:\t\t[%d, %d]\n",
                ValueCaps->Range.StringMin, ValueCaps->Range.StringMax);
            printf("ValueCaps Designator:\t\t[%d, %d]\n",
                ValueCaps->Range.DesignatorMin, ValueCaps->Range.DesignatorMax);
            printf("ValueCaps DataIndex:\t\t[%d, %d]\n",
                ValueCaps->Range.DataIndexMin, ValueCaps->Range.DataIndexMax);
        } else {
            printf(
                "ValueCaps Usage:\t\t%d\n",
                ValueCaps->NotRange.Usage);

            if (LogConfig.Hid.UsageText) {
                UsageText = getHidUsageText(ValueCaps->UsagePage, ValueCaps->NotRange.Usage);
                if (UsageText != NULL) {
                    printf("ValueCaps Usage Text:\t\t%s\n", UsageText);
                    free(UsageText);
                }
            }

            printf(
                "ValueCaps StringIndex:\t\t%d\n",
                ValueCaps->NotRange.StringIndex);
            printf(
                "ValueCaps DesignatorIndex:\t%d\n",
                ValueCaps->NotRange.DesignatorIndex);
            printf(
                "ValueCaps DataIndex:\t\t%d\n",
                ValueCaps->NotRange.DataIndex);
        }
    }
}

PHIDP_PREPARSED_DATA
GetHidPreparsedData(
    _In_ HANDLE Handle
    )
{
    UINT BytesReturned;
    UINT RequiredSize;
    PHIDP_PREPARSED_DATA PreparsedData;

    RequiredSize = 0;
    BytesReturned = GetRawInputDeviceInfo(
        Handle,
        RIDI_PREPARSEDDATA,
        NULL,
        &RequiredSize);

    if (BytesReturned != 0 || RequiredSize == 0) {
        printf(
            "GetRawInputDeviceInfo failed, RequiredSize = 0x%x, GLE = 0x%x\n",
            RequiredSize,
            GetLastError());
        return NULL;
    }

    PreparsedData = malloc(RequiredSize);
    if (PreparsedData == NULL) {
        FailFast(E_OUTOFMEMORY);
        return NULL;
    }

    BytesReturned = GetRawInputDeviceInfo(
        Handle,
        RIDI_PREPARSEDDATA,
        PreparsedData,
        &RequiredSize);

    if (BytesReturned <= 0) {
        printf(
            "GetRawInputDeviceInfo failed, BytesReturned = 0x%x, GLE = 0x%x\n",
            BytesReturned,
            GetLastError());
        return NULL;
    }

    return PreparsedData;
}

PREGISTERED_HID_DEVICE_INFO
RegisterHidDevice(
    _In_ HANDLE Handle,
    _In_ LPWSTR InstancePath
    )
{
    UINT Count;
    PBUTTON_CAP_STATE Buttons;
    HIDP_CAPS HidCaps;
    PREGISTERED_HID_DEVICE_INFO HidInfo = NULL;
    HANDLE HidHandle = INVALID_HANDLE_VALUE;
    NTSTATUS Status;
    USHORT Length;
    USAGE Usage;

    HidInfo = malloc(sizeof(*HidInfo));
    if (HidInfo == NULL) {
        FailFast(E_OUTOFMEMORY);
        goto Exit;
    }
    ZeroMemory(HidInfo, sizeof(*HidInfo));

    HidInfo->TimerList = InitTimerList();
    if (HidInfo->TimerList == NULL) {
        FailFast(E_OUTOFMEMORY);
        goto Exit;
    }

    HidInfo->PreparsedData = GetHidPreparsedData(Handle);
    if (HidInfo->PreparsedData == NULL) {
        goto Exit;
    }

    // device caps
    Status = HidP_GetCaps(HidInfo->PreparsedData, &HidCaps);
    if (Status != HIDP_STATUS_SUCCESS) {
        printf(
            "HidP_GetCaps failed, Device %S, GLE = 0x%x\n",
            InstancePath,
            GetLastError());
        goto Exit;
    }
    CopyMemory(&HidInfo->HidCaps, &HidCaps, sizeof(HidCaps));

    // button caps
    if (0 < HidCaps.NumberInputButtonCaps) {
        HidInfo->ButtonCaps = malloc(
            sizeof(HidInfo->ButtonCaps[0]) * HidCaps.NumberInputButtonCaps);
        if (HidInfo->ButtonCaps == NULL) {
            FailFast(E_OUTOFMEMORY);
            goto Exit;
        }

        HidInfo->ButtonCapStates = malloc(
            sizeof(HidInfo->ButtonCapStates[0]) * HidCaps.NumberInputButtonCaps);
        if (HidInfo->ButtonCapStates == NULL) {
            FailFast(E_OUTOFMEMORY);
            goto Exit;
        }

        Length = HidCaps.NumberInputButtonCaps;
        Status = HidP_GetButtonCaps(
            HidP_Input, 
            HidInfo->ButtonCaps,
            &Length,
            HidInfo->PreparsedData
            );
        
        if (Status != HIDP_STATUS_SUCCESS) {
            printf(
                "HidP_GetButtonCaps failed, Device %S, GLE = 0x%x\n",
                InstancePath,
                GetLastError());
            goto Exit;
        }

        // process each button cap
        // (which could be a range of buttons or a single button)
        for (Count = 0; Count < Length; Count += 1) {
            HidInfo->ButtonCapStates[Count] = malloc(sizeof(BUTTON_CAP_STATE));
            if (HidInfo->ButtonCapStates[Count] == NULL) {
                FailFast(E_OUTOFMEMORY);
                goto Exit;
            }

            Buttons = HidInfo->ButtonCapStates[Count];

            if (HidInfo->ButtonCaps[Count].IsRange) {
                Buttons->Length =
                    HidInfo->ButtonCaps[Count].Range.UsageMax -
                    HidInfo->ButtonCaps[Count].Range.UsageMin +
                    1;
                Buttons->UsageMin = HidInfo->ButtonCaps[Count].Range.UsageMin;
            } else {
                Buttons->Length = 1;
                Buttons->UsageMin = HidInfo->ButtonCaps[Count].NotRange.Usage;
            }

            // array of booleans to keep track of state
            Buttons->PreviousState = malloc(sizeof(BOOLEAN) * Buttons->Length);
            ZeroMemory(Buttons->PreviousState, sizeof(BOOLEAN) * Buttons->Length);
            Buttons->CurrentState = malloc(sizeof(BOOLEAN) * Buttons->Length);
            ZeroMemory(Buttons->CurrentState, sizeof(BOOLEAN) * Buttons->Length);

            // array of strings for each button
            Buttons->UsageText = malloc(sizeof(PWSTR) * Buttons->Length);
            
            // get text for each button
            if (HidInfo->ButtonCaps[Count].IsRange) {
                for (Usage = 0;
                    Usage < Buttons->Length;
                    Usage += 1) {

                    Buttons->UsageText[Usage] = getHidUsageText(
                        HidInfo->ButtonCaps[Count].UsagePage,
                        (Usage + Buttons->UsageMin));
                }
            } else {
                Buttons->UsageText[0] = getHidUsageText(
                    HidInfo->ButtonCaps[Count].UsagePage,
                    Buttons->UsageMin);
            }
        }
    }

    // input value caps
    if (0 < HidCaps.NumberInputValueCaps) {
        HidInfo->ValueCaps = malloc(
            sizeof(HidInfo->ButtonCaps[0]) * HidCaps.NumberInputValueCaps);

        if (HidInfo->ValueCaps == NULL) {
            FailFast(E_OUTOFMEMORY);
            goto Exit;
        }

        Length = HidCaps.NumberInputValueCaps;
        Status = HidP_GetValueCaps(
            HidP_Input, 
            HidInfo->ValueCaps,
            &Length,
            HidInfo->PreparsedData
            );
        
        if (Status != HIDP_STATUS_SUCCESS) {
            printf(
                "HidP_GetValueCaps failed, Device %S, GLE = 0x%x\n",
                InstancePath,
                GetLastError());
            goto Exit;
        }
    }

    // open a file handle
    HidHandle = CreateFile(
        InstancePath,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (HidHandle == INVALID_HANDLE_VALUE) {
        if (LogConfig.VerboseError) {
            printf(
                "CreateFile failed, Device %S, GLE = 0x%x\n",
                InstancePath,
                GetLastError());
        }
    } else {
        if (HidD_GetManufacturerString(
                HidHandle,
                HidInfo->Manufacturer,
                sizeof(HidInfo->Manufacturer)) == FALSE) {
            if (LogConfig.VerboseError) {
                printf(
                    "HidD_GetManufacturerString failed, Device %S, GLE = 0x%x\n",
                    InstancePath,
                    GetLastError());
            }
        }
        if (HidD_GetProductString(
                HidHandle,
                HidInfo->Product,
                sizeof(HidInfo->Product)) == FALSE) {
            if (LogConfig.VerboseError) {
                printf(
                    "HidD_GetProductString failed, Device %S, GLE = 0x%x\n",
                    InstancePath,
                    GetLastError());
            }
        }
        if (HidD_GetSerialNumberString(
                HidHandle,
                HidInfo->SerialNumber,
                sizeof(HidInfo->SerialNumber)) == FALSE) {
            if (LogConfig.VerboseError) {
                printf(
                    "HidD_GetSerialNumberString failed, Device %S, GLE = 0x%x\n",
                    InstancePath,
                    GetLastError());
            }
        }

        CloseHandle(HidHandle);
        HidHandle = INVALID_HANDLE_VALUE;
    }

Exit:
    return HidInfo;
}

VOID
FreeButtonCapState(
    _In_ _Frees_ptr_ PBUTTON_CAP_STATE ButtonCapState
    )
{
    UINT Count;

    // free the boolean array
    if (ButtonCapState->PreviousState != NULL) {
        free(ButtonCapState->PreviousState);
        ButtonCapState->PreviousState = NULL;
    }
    if (ButtonCapState->CurrentState != NULL) {
        free(ButtonCapState->CurrentState);
        ButtonCapState->CurrentState = NULL;
    }
    if (ButtonCapState->UsageText != NULL) {
        // free the strings
        for (Count = 0; Count < ButtonCapState->Length; Count += 1) {
            free(ButtonCapState->UsageText[Count]);
            ButtonCapState->UsageText[Count] = NULL;
        }
        // free the array of pointers to strings
        free(ButtonCapState->UsageText);
        ButtonCapState->UsageText = NULL;
    }
    free(ButtonCapState);
}

VOID
FreeRegisteredHidDevice(
    _In_ _Frees_ptr_ PREGISTERED_HID_DEVICE_INFO HidDeviceInfo
    )
{
    UINT Count;

    if (HidDeviceInfo->TimerList != NULL) {
        FreeTimerList(HidDeviceInfo->TimerList);
        HidDeviceInfo->TimerList = NULL;
    }
    if (HidDeviceInfo->PreparsedData != NULL) {
        free(HidDeviceInfo->PreparsedData);
        HidDeviceInfo->PreparsedData = NULL;
    }
    if (HidDeviceInfo->ButtonCaps != NULL) {
        free(HidDeviceInfo->ButtonCaps);
        HidDeviceInfo->ButtonCaps = NULL;
    }

    if (HidDeviceInfo->ButtonCapStates != NULL) {
        // free the contents of the array
        for (Count = 0;
            Count < HidDeviceInfo->HidCaps.NumberInputButtonCaps;
            Count += 1) {
            FreeButtonCapState(HidDeviceInfo->ButtonCapStates[Count]);
            HidDeviceInfo->ButtonCapStates[Count] = NULL;
        }
        // free the array of pointers
        free(HidDeviceInfo->ButtonCapStates);
        HidDeviceInfo->ButtonCapStates = NULL;
    }

    free(HidDeviceInfo);
}

BOOLEAN
CheckHidEvent(
    _In_ PRAWHID Hid
    )
{
    return TRUE;
}

VOID
ProcessHidEvent(
    _In_ PREGISTERED_DEVICE Device,
    _In_ PRAWHID Hid
    )
{
    UINT ButtonCount;
    PHIDP_BUTTON_CAPS ButtonCaps = NULL;
    PBUTTON_CAP_STATE ButtonCapState = NULL;
    UINT Count;
    NTSTATUS Status;
    USAGE Usage = 0;
    PUSAGE Usages = NULL;
    UINT UsageCount;
    PCHAR UsageText = NULL;
    ULONG Identifier;
    UINT TimeInMs;

    if (LogConfig.LogEvents) {
        DumpHidEvent(Device, Hid);
    }

    if (!LogConfig.LogChatter) {
        return;
    }

    for (Count = 0;
        Count < Device->HidDeviceInfo->HidCaps.NumberInputButtonCaps;
        Count += 1) {

        ButtonCaps = &Device->HidDeviceInfo->ButtonCaps[Count];
        ButtonCapState = Device->HidDeviceInfo->ButtonCapStates[Count];
        if (ButtonCapState->Length <= 0) {
            continue;
        }

        Usages = malloc(sizeof(Usages[0]) * ButtonCapState->Length);
        if (Usages == NULL) {
            FailFast(E_OUTOFMEMORY);
            break;
        }

        ButtonCount = ButtonCapState->Length;
        Status = HidP_GetUsages(
            HidP_Input,
            ButtonCaps->UsagePage,
            ButtonCaps->LinkCollection,
            Usages,
            &ButtonCount,
            Device->HidDeviceInfo->PreparsedData,
            Hid->bRawData,
            Hid->dwSizeHid);

        if (Status == HIDP_STATUS_SUCCESS) {
            // construct current state array based on the result of ON buttons
            ZeroMemory(
                ButtonCapState->CurrentState,
                sizeof(ButtonCapState->CurrentState[0]) * ButtonCapState->Length);

            for (UsageCount = 0; UsageCount < ButtonCount; UsageCount += 1) {
#pragma warning(suppress: 6385)
                Usage = Usages[UsageCount];
                ButtonCapState->CurrentState[Usage - ButtonCapState->UsageMin] =
                    TRUE;
            }

            // compare current vs. previous state and detect rising / falling
            // edges
            for (UsageCount = 0;
                UsageCount < ButtonCapState->Length;
                UsageCount += 1) {

                Usage = ButtonCapState->UsageMin + UsageCount;
                Identifier = (Count << 16) || (Usage);

                // OFF -> ON rising edge, start the timer
                if (!ButtonCapState->PreviousState[UsageCount] &&
                    ButtonCapState->CurrentState[UsageCount]) {

                    StartTimer(
                        Device->HidDeviceInfo->TimerList, Identifier, FALSE);
                
                // ON -> OFF falling edge, stop timer
                } else if (ButtonCapState->PreviousState[UsageCount] &&
                    !ButtonCapState->CurrentState[UsageCount]) {

                    TimeInMs = StopTimerAndGetMsElapsed(
                        Device->HidDeviceInfo->TimerList, Identifier);

                    UsageText = ButtonCapState->UsageText[UsageCount];

                    PrintLogTimeStamp();
                    printf(
                        "Duration (%*s): %*d ms",
                        16, UsageText,
                        6, TimeInMs);

                    if (TimeInMs <= AppConfig.GlitchDurationInMs) {
                        printf("\t***GLITCH!!***");
                    }
                    printf("\n");
                }
            }

            // update previous state array for next time
            CopyMemory(
                ButtonCapState->PreviousState,
                ButtonCapState->CurrentState,
                sizeof(ButtonCapState->CurrentState[0]) * ButtonCapState->Length);
        }

        free(Usages);
    }
}

VOID
DumpHidEvent(
    _In_ PREGISTERED_DEVICE Device,
    _In_ PRAWHID Hid
    )
{
    PHIDP_BUTTON_CAPS ButtonCaps;
    PBUTTON_CAP_STATE ButtonCapState;
    PHIDP_VALUE_CAPS ValueCaps;
    UINT ButtonCount;
    UINT ValueCount;
    UINT Count;
    NTSTATUS Status;
    USAGE Usage;
    PUSAGE Usages;
    UINT UsageCount;
    PCHAR UsageText;
    ULONG Value;

    for (Count = 0;
        Count < Device->HidDeviceInfo->HidCaps.NumberInputValueCaps;
        Count += 1) {

        ValueCaps = &Device->HidDeviceInfo->ValueCaps[Count];

        if (ValueCaps->IsRange) {
            ValueCount =
                ValueCaps->Range.UsageMax - ValueCaps->Range.UsageMin + 1;
        } else {
            ValueCount = 1;
        }

        if (ValueCount <= 0) {
            continue;
        }

        for (UsageCount = 0; UsageCount < ValueCount; UsageCount += 1) {
            if (ValueCaps->IsRange) {
                Usage = ValueCaps->Range.UsageMin + UsageCount;
            } else {
                Usage = ValueCaps->NotRange.Usage;
            }

            Status = HidP_GetUsageValue(
                HidP_Input,
                ValueCaps->UsagePage,
                ValueCaps->LinkCollection,
                Usage,
                &Value,
                Device->HidDeviceInfo->PreparsedData,
                Hid->bRawData,
                Hid->dwSizeHid);

            if (Status != HIDP_STATUS_SUCCESS) {
                continue;
            }

            UsageText = getHidUsageText(ValueCaps->UsagePage, Usage);
            printf("Value:\t\t[%s] 0x%x\n", UsageText, Value);
            if (UsageText != NULL) {
                free(UsageText);
            }
        }
    }

    for (Count = 0;
        Count < Device->HidDeviceInfo->HidCaps.NumberInputButtonCaps;
        Count += 1) {

        ButtonCaps = &Device->HidDeviceInfo->ButtonCaps[Count];
        ButtonCapState = Device->HidDeviceInfo->ButtonCapStates[Count];
        if (ButtonCapState->Length <= 0) {
            continue;
        }

        Usages = malloc(sizeof(Usages[0]) * ButtonCapState->Length);
        if (Usages == NULL) {
            FailFast(E_OUTOFMEMORY);
            break;
        }

        ButtonCount = ButtonCapState->Length;
        Status = HidP_GetUsages(
            HidP_Input,
            ButtonCaps->UsagePage,
            ButtonCaps->LinkCollection,
            Usages,
            &ButtonCount,
            Device->HidDeviceInfo->PreparsedData,
            Hid->bRawData,
            Hid->dwSizeHid);

        if (Status == HIDP_STATUS_SUCCESS) {
            for (UsageCount = 0; UsageCount < ButtonCount; UsageCount += 1) {
#pragma warning(suppress: 6385)
                Usage = Usages[UsageCount];
                UsageText =
                    ButtonCapState->UsageText[Usage - ButtonCapState->UsageMin];

                printf("Button:\t\t%d (%s)\n", Usage, UsageText);
            }
        }

        free(Usages);
    }
}