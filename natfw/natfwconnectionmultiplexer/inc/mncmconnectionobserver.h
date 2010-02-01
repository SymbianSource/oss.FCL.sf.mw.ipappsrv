/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Observer interface used to receive notifications from
*                Connection Multiplexer.
*
*/




#ifndef M_MNCMCONNECTIONOBSERVER_H
#define M_MNCMCONNECTIONOBSERVER_H

#include <e32base.h>


/**
 *  Observer interface used to receive notifications from
 *  Connection Multiplexer
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
class MNcmConnectionObserver
    {

public:

/**  Types of connection notifies */
enum TConnectionNotifyType
    {
    EConnectionRemoved      = 1,
    EConnectionError        = 2,
    EReceivingActivated     = 3,
    EReceivingDeactivated   = 4,
    ESendingActivated       = 5,
    ESendingDeactivated     = 6,
    ETcpConnSetupCompleted  = 7,
    EFirstMediaPacketSent   = 8
    };
    
    /**
     * Called by Connection Multiplexer when actions for connection completes
     *
     * @since    S60 v3.2
     * @param    aStreamId           The ID identifying stream
     * @param    aConnectionId       The ID identifying connection
     * @param    aType               Type of notify
     * @param    aError              Error code
     * @return   void
     */
    virtual void ConnectionNotify( TUint aStreamId, TUint aConnectionId,
        TConnectionNotifyType aType, TInt aError ) = 0;

    };


#endif // M_MNCMCONNECTIONOBSERVER_H
