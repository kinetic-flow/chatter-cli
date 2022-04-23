#include "chatter.h"

typedef struct _QPC_TIMER {
    UINT Identifier;
    LARGE_INTEGER StartTime;
} QPC_TIMER, *PQPC_TIMER;

list_node_t *
FindTimer(
    _Inout_ list_t *TimerList,
    _In_ UINT Identifier
    );

LARGE_INTEGER QpcFrequency;

VOID
InitTimerSupport(
    VOID
    )
{
    QueryPerformanceFrequency(&QpcFrequency); 
}

list_t *
InitTimerList(
    VOID
    )
{
    return list_new();
}

VOID
FreeTimerList(
    _In_ _Frees_ptr_ list_t *TimerList
    )
{
    list_destroy(TimerList);
}

VOID
StartTimer(
    _Inout_ list_t *TimerList,
    _In_ UINT Identifier,
    _In_ BOOLEAN CheckExisting
    )
{

    PQPC_TIMER Timer;
    list_node_t *Node;

    if (CheckExisting) {
        Node = FindTimer(TimerList, Identifier);
        if (Node != NULL) {
            return;
        }
    }

    Timer = malloc(sizeof(*Timer));
    if (Timer == NULL) {
        FailFast(E_OUTOFMEMORY);
    }
    
    Timer->Identifier = Identifier;
    QueryPerformanceCounter(&Timer->StartTime);
    list_rpush(TimerList, list_node_new(Timer));
}

list_node_t *
FindTimer(
    _Inout_ list_t *TimerList,
    _In_ UINT Identifier
    )
{
    PQPC_TIMER Timer;
    list_node_t *node;
    list_iterator_t *it;

    it = list_iterator_new(TimerList, LIST_HEAD);
    while (node = list_iterator_next(it)) {
        Timer = node->val;
        if (Timer->Identifier == Identifier) {
            break;
        }
    }
    list_iterator_destroy(it);
    return node;
}

UINT
StopTimerAndGetMsElapsed(
    _Inout_ list_t *TimerList,
    _In_ UINT Identifier
    )
{
    LARGE_INTEGER ElapsedMicroseconds = {0};
    LARGE_INTEGER EndingTime = {0};
    PQPC_TIMER Timer = NULL;
    list_node_t *Node;

    // capture the end time first
    QueryPerformanceCounter(&EndingTime);

    // find the node in list
    Node = FindTimer(TimerList, Identifier);
    if (Node) {
        Timer = Node->val;

        // calculate the delta
        ElapsedMicroseconds.QuadPart =
            EndingTime.QuadPart - Timer->StartTime.QuadPart;
        ElapsedMicroseconds.QuadPart *= 1000000;
        ElapsedMicroseconds.QuadPart /= QpcFrequency.QuadPart;

        // remove from list
        free(Node->val);
        Node->val = NULL;
        list_remove(TimerList, Node);
        Node = NULL;
        
        return (UINT)(ElapsedMicroseconds.QuadPart / 1000);
    }

    return MAXUINT;
}