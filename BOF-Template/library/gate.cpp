#include <windows.h>

#include "../beacon_gate.h"
#include "../debug.h"
#include "../sleepmask.h"
#include "../sleepmask-vs.h"

/**
* A wrapper around BeaconGate to handle masking/unmasking Beacon
*
* @param info A pointer to a SLEEPMASK_INFO structure
* @param functionCall A pointer to a FUNCTION_CALL structure
*/
void BeaconGateWrapper(PSLEEPMASK_INFO info, PFUNCTION_CALL functionCall) {
    if (functionCall->bMask == TRUE) {
        MaskBeacon(&info->beacon_info);
    }

    BeaconGate(functionCall);

    if (functionCall->bMask == TRUE) {
        UnMaskBeacon(&info->beacon_info);
    }

    return;
}

/**
* Execute BeaconGate.
*
* @param functionCall A pointer to a FUNCTION_CALL structure
*/
void BeaconGate(PFUNCTION_CALL functionCall) {
    ULONG_PTR retValue = 0;
    
    /** 
    * Call appropriate function pointer based on number of args.
    *
    * Note: This is not a switch statement because it adds linker
    * errors. 
    */
    if (functionCall->numOfArgs == 0) {
        retValue = beaconGate(00)();
    }
    else if (functionCall->numOfArgs == 1) {
        retValue = beaconGate(01)(arg(0));
    }
    else if (functionCall->numOfArgs == 2) {
        retValue = beaconGate(02)(arg(0), arg(1));
    }
    else if (functionCall->numOfArgs == 3) {
        retValue = beaconGate(03) (arg(0), arg(1), arg(2));
    }
    else if (functionCall->numOfArgs == 4) {
        retValue = beaconGate(04) (arg(0), arg(1), arg(2), arg(3));
    }
    else if (functionCall->numOfArgs == 5) {
        retValue = beaconGate(05) (arg(0), arg(1), arg(2), arg(3), arg(4));
    }
    else if (functionCall->numOfArgs == 6) {
        retValue = beaconGate(06) (arg(0), arg(1), arg(2), arg(3), arg(4), arg(5));
    }
    else if (functionCall->numOfArgs == 7) {
        retValue = beaconGate(07) (arg(0), arg(1), arg(2), arg(3), arg(4), arg(5), arg(6));
    }
    else if (functionCall->numOfArgs == 8) {
        retValue = beaconGate(08) (arg(0), arg(1), arg(2), arg(3), arg(4), arg(5), arg(6), arg(7));
    }
    else if (functionCall->numOfArgs == 9) {
        retValue = beaconGate(09) (arg(0), arg(1), arg(2), arg(3), arg(4), arg(5), arg(6), arg(7), arg(8));
    }
    else if (functionCall->numOfArgs == 10) {
        retValue = beaconGate(10) (arg(0), arg(1), arg(2), arg(3), arg(4), arg(5), arg(6), arg(7), arg(8), arg(9));
    }

    functionCall->retValue = retValue;

    return;
}
