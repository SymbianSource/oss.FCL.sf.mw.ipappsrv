/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:    
*
*/




#ifndef M_MICENATPLUGINEVENTOBS_H
#define M_MICENATPLUGINEVENTOBS_H

#include <e32base.h>

class CNATFWPluginApi;

/**
 *  Observer interface for clients interested to receive NAT plugin events.
 *
 *  @lib icecandidatehandler.dll
 *  @since S60 v3.2
 */
class MIceNatPluginEventObs
    {

public:

    /**  NAT plugin events */
    enum TNatPluginEvent
        {
        EFirstPluginEventInd    = 0,
        EServerConnected        = 0,
        ELocalCandidateFound    = 1,
        EFetchingCompleted      = 2,
        ETcpConnSetupCompleted  = 3,
        EReceivingActivated     = 4,
        EReceivingDeactivated   = 5,
        ESendingActivated       = 6,
        ESendingDeactivated     = 7,
        ECandidatePairFound     = 8,
        EConnChecksCompleted    = 9,
        EGeneralError           = 10,
        ELastPluginEventInd     = 10
        };
    
    /**
     * Notifies about plugin event.
     *  
     * @since   S60 v3.2
     * @param   aPlugin         The plugin raising the event
     * @param   aStreamId       The stream identifier
     * @param   aEventCode      The code specifying event
     * @param   aErrorCode      The error code associated with the event
     * @param   aEventData      Optional event data
     */
    virtual void PluginEventOccured( const CNATFWPluginApi* aPlugin,
        TUint aStreamId, TNatPluginEvent aEventCode, TInt aErrorCode, 
        TAny* aEventData, TUint aStreamConnectionId = 0,
        TUint aComponentId = 0, TBool aIPv6After = EFalse ) = 0;

    };


#endif // M_MICENATPLUGINEVENTOBS_H
