#pragma once

#include "chatter.h"

VOID
InitTimerSupport(
    VOID
    );

list_t *
InitTimerList(
    VOID
    );

VOID
FreeTimerList(
    _In_ _Frees_ptr_ list_t *TimerList
    );

VOID
StartTimer(
    _Inout_ list_t *TimerList,
    _In_ UINT Identifier,
    _In_ BOOLEAN CheckExisting
    );

UINT
StopTimerAndGetMsElapsed(
    _Inout_ list_t *TimerList,
    _In_ UINT Identifier
    );