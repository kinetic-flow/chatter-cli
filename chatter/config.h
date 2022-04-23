#pragma once

#include "chatter.h"

typedef struct _APP_CONFIG {
    BOOLEAN UseVendorSpecificHidDevices;
    UINT GlitchDurationInMs;
} APP_CONFIG, *PAPP_CONFIG;

typedef struct _LOG_CONFIG {
    BOOLEAN LogChatter;
    BOOLEAN LogEvents;
    BOOLEAN VerboseError;
    struct {
        BOOLEAN Movement;
        BOOLEAN ButtonClicks;
        BOOLEAN Verbose;
    } Mouse;
    struct {
        BOOLEAN Verbose;
    } Keyboard;
    struct {
        BOOLEAN UsageText;
    } Hid;
} LOG_CONFIG, *PLOG_CONFIG;

extern LOG_CONFIG LogConfig;
extern APP_CONFIG AppConfig;
