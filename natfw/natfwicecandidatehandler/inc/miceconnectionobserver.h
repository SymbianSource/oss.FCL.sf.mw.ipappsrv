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




#ifndef M_MICECONNECTIONOBSERVER_H
#define M_MICECONNECTIONOBSERVER_H

#include <e32base.h>

class CIceConnection;

/**
 *  ICE connection observer interface.
 *
 *  @lib icecandidatehandler.dll
 *  @since S60 v3.2
 */
class MIceConnectionObserver
    {

public:

    /**  Connection notify type */
    enum TNotifyType
        {
        ENotifyUnknown              = 0,
        ENotifyConnectionRemoved    = 1,
        ENotifyConnectionError      = 2,
        ENotifyRecvActivated        = 3,
        ENotifyRecvDeactivated      = 4,
        ENotifySendActivated        = 5,
        ENotifySendDeactivated      = 6,
        ENotifyFirstMediaPacketSent = 7
        };

    /**
     * Notifies about connection state changes.
     *
     * @since   S60 v3.2
     * @param   aConnection         Notifying connection
     * @param   aType               Type of notify
     * @param   aError              Error code
     */
    virtual void ConnectionNotify( CIceConnection& aConnection,
        TNotifyType aType, TInt aError ) = 0;
    };


#endif // M_MICECONNECTIONOBSERVER_H