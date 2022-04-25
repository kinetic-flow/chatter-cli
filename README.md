# chatter-cli

Tool for testing HID input devices (keyboard, mice, joystick, gamepad, touch screen, digitizers...)

## Usage

This is a command line program. Launch from a Windows command window:

I have seen some issues with Windows Terminal app (issues with scrolling through fast messages, occasional crashing), so if you have problems, try a traditional cmd window.

To start chatter test mode:

    chatter.exe -c
    chatter.exe --chatter

To show a list of raw input devices: 

    chatter.exe -l
    chatter.exe --list

To monitor all HID input events:

    chatter.exe -m
    chatter.exe --monitor

It is possible to mix and match the modes described above.

### Extra options

* Specify -u (--cursor) to include cursor movement events. These are off by default to reduce noise.
* Specify -v (--verbose) to include verbose error message output. Try this if you are seeing any failures or unexpected behavior.
* Specify -e (--vendor) to include devices with vendor-specific usage pages. These are skipped by default, but this flag will cause them to be included.
* Specify -d (--duration) with an integer value to set the threshold in which "GLITCH" text is displayed. Default value is 15ms.

e.g., 

    chatter.exe -c -d 20

## Details

GetRawInputDeviceList and GetRawInputDeviceInfo are used to obtain a list of raw input devices from Win32.

HID APIs in HID.dll are used to obtain HID device information (HidP_GetUsages, HidP_GetUsageValue, HidP_GetButtonCaps, HidP_GetValueCaps, and so on).

## Examples

    chatter.exe --chatter
    
    Monitoring raw input from 17 device(s)
    Press CTRL+C to exit any time.

    [02:06:36.295] Duration (A               ):     88 ms
    [02:06:36.594] Duration (F               ):    103 ms
    [02:06:36.615] Duration (I               ):     97 ms
    [02:06:36.621] Duration (J               ):     95 ms
    [02:06:37.428] Duration (Mouse button 1  ):     77 ms
    [02:06:38.751] Duration (Mouse button 1  ):     71 ms
    [02:07:44.240] Duration (Button 4        ):     15 ms   ***GLITCH!!***
    [02:06:39.383] Duration (Button 1        ):    120 ms
    [02:06:39.748] Duration (Button 2        ):    144 ms
    [02:06:39.944] Duration (Button 2        ):    120 ms
    [02:06:41.123] Duration (Mouse button 1  ):     69 ms
    CTRL + C detected, exiting...

---

    chatter.exe --list

    [Device #0 - KEYBOARD (ID: 340aa1)]
    \\?\Microsoft Keyboard RID\0
    dwType                  : 0x0
    dwSubType               : 0
    dwKeyboardMode          : 0
    dwNumberOfFunctionKeys  : 0
    dwNumberOfFunctionKeys  : 0
    dwNumberOfKeysTotal     : 0

    [Device #1 - HID (ID: 200cb)]
    \\?\HID#VID_045E&PID_02FF&IG_00#8&3405f702&0&0000#{4d1e55b2-f16f-11cf-88cb-001111000030}
    dwVendorId                    : 0x45e
    dwProductId                   : 0x2ff
    dwVersionNumber               : 0x0
    usUsagePage                   : 0x1
    usUsage                       : 0x5
    Usage Text                    : Game Pad
    ---
    Manufacturer String           : 
    Product String                : Controller (Xbox One For Windows)
    SerialNumber String           : 
    ---
    HID Usage                     : 0x5
    HID UsagePage                 : 0x1
    HID InputReportByteLength     : 0x10
    HID OutputReportByteLength    : 0x0
    HID FeatureReportByteLength   : 0x0
    HID NumberLinkCollectionNodes : 4
    HID NumberInputButtonCaps     : 1
    HID NumberInputValueCaps      : 6
    HID NumberInputDataIndices    : 22
    HID NumberOutputButtonCaps    : 0
    HID NumberOutputValueCaps     : 0
    HID NumberOutputDataIndices   : 0
    HID NumberFeatureButtonCaps   : 0
    HID NumberFeatureValueCaps    : 0
    HID NumberFeatureDataIndices  : 0
    ---
    ButtonCaps UsagePage          : 0x9
    ButtonCaps ReportID           : 0
    ButtonCaps BitField           : 2
    ButtonCaps LinkCollection     : 0
    ButtonCaps LinkUsage          : 5
    ButtonCaps LinkUsage          : 5
    ButtonCaps ...                : IsRange IsAbsolute 
    ButtonCaps ReportCount        : 1
    ButtonCaps Usage              : [1, 16]
    ButtonCaps Usage Text         : Button 1, Button 2, Button 3, Button 4, Button 5, Button 6, Button 7, Button 8, Button 9, Button 10, Button 11, Button 12, Button 13, Button 14, Button 15, Button 16
    ButtonCaps String             : [0, 0]
    ButtonCaps Designator         : [0, 0]
    ButtonCaps DataIndex          : [5, 20]
    ---
    ValueCaps UsagePage           : 0x1
    ValueCaps ReportID            : 0
    ValueCaps BitField            : 66
    ValueCaps LinkCollection      : 0
    ValueCaps LinkUsage           : 5
    ValueCaps LinkUsagePage       : 1
    ValueCaps ReportID            : 0
    ValueCaps ...                 : IsAbsolute HasNull 
    ValueCaps BitSize             : 4
    ValueCaps ReportCount         : 1
    ValueCaps UnitsExp            : 0x0
    ValueCaps Units               : 0xe
    ValueCaps LogicalMin          : 0x1
    ValueCaps LogicalMax          : 0x8
    ValueCaps PhysicalMin         : 0x0
    ValueCaps PhysicalMax         : 0x103b
    ValueCaps Usage               : 57
    ValueCaps StringIndex         : 0
    ValueCaps DesignatorIndex     : 0
    ValueCaps DataIndex           : 21

    [Device #2 - KEYBOARD (ID: 1004c)]
    \\?\HID#VID_1532&PID_0243&MI_00#9&fc640ce&0&0000#{884b96c3-56ef-11d1-bc8c-00a0c91405dd}
    dwType                  : 0x51 HID_KEYBOARD_OR_UNKNOWN_TYPE
    dwSubType               : 0
    dwKeyboardMode          : 1
    dwNumberOfFunctionKeys  : 12
    dwNumberOfFunctionKeys  : 12
    dwNumberOfKeysTotal     : 264

---

    chatter.exe --monitor
        
    Monitoring raw input from 17 device(s)
    Press CTRL+C to exit any time.

    [02:10:39.444] [Device Type: KEYBOARD, Device ID: 0x10052]
    MakeCode    : 0x1c Enter 
    VKey        : 0xd VK_RETURN
    Message     : 0x101 WM_KEYUP 

    [02:10:40.687] [Device Type: HID, Device ID: 0x200cb]
    Value   : [Y] 0x8217
    Value   : [X] 0x8c86
    Value   : [Ry] 0x84aa
    Value   : [Rx] 0x7db0
    Value   : [Z] 0x8000
    Value   : [Hat switch] 0x0
    Button  : 2 (Button 2)

    [02:10:40.758] [Device Type: HID, Device ID: 0x200cb]
    Value   : [Y] 0x8217
    Value   : [X] 0x8c86
    Value   : [Ry] 0x84aa
    Value   : [Rx] 0x7db0
    Value   : [Z] 0x8000
    Value   : [Hat switch] 0x0

    [02:10:41.530] [Device Type: KEYBOARD, Device ID: 0x10052]
    MakeCode    : 0x1d Ctrl 
    VKey        : 0x11 VK_CONTROL
    Message     : 0x100 WM_KEYDOWN 

    [02:10:41.645] [Device Type: KEYBOARD, Device ID: 0x10052]
    MakeCode    : 0x1d Ctrl 
    VKey        : 0x11 VK_CONTROL
    Message     : 0x101 WM_KEYUP 

    [02:10:42.378] [Device Type: MOUSE, Device ID: 0x1004a]
    usButtonFlags   : 0x1 RI_MOUSE_LEFT_BUTTON_DOWN 

    [02:10:42.476] [Device Type: MOUSE, Device ID: 0x1004a]
    usButtonFlags   : 0x2 RI_MOUSE_LEFT_BUTTON_UP 

    CTRL + C detected, exiting...

## Compiling

Use MSVC with Visual Studio 2022 Community Edition. Only x86 target has been tested, but in theory other architectures should work just fine when compiled.
