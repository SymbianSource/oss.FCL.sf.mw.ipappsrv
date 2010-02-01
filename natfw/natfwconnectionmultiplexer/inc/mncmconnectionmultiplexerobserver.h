/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef M_MNCMCONNECTIONMULTIPLEXEROBSERVER_H
#define M_MNCMCONNECTIONMULTIPLEXEROBSERVER_H

#include <e32base.h>

class TInetAddr;

/**
 *  Observer interface used to receive notifications from
 *  Connection Multiplexer
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
class MNcmConnectionMultiplexerObserver
    {

public:

    /**  Types of notifies */
    enum TNotifyType
        {
        ESessionCreated         = 1,
        EReceivingActivated     = 2,
        EReceivingDeactivated   = 3,
        ESendingActivated       = 4,
        ESendingDeactivated     = 5,
        EStreamError            = 6
        };

    /**
     * Called by Connection Multiplexer when actions completes
     *
     * @since   S60 v3.2
     * @param   aSessionId      The ID identifying session
     * @param   aStreamId       The ID identifying stream
     * @param   aType           Type of notify
     * @param   aError          Error code. KErrNone if successful.
     * @return void
     */
    virtual void Notify( TUint aSessionId, TUint aStreamId,
        TNotifyType aType, TInt aError ) = 0;       
    };


#endif // M_MNCMCONNECTIONMULTIPLEXEROBSERVER_H
