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
* Description:    Observer for CMultiplexerConnection
*
*/




#ifndef M_MNCMMULTIPLEXERCONNECTIONOBSERVER_H
#define M_MNCMMULTIPLEXERCONNECTIONOBSERVER_H

#include <e32base.h>

#include "ncmconnectionmultiplexer.hrh"

/**
 *  Observer for CNcmConnection
 *
 *  Allow user of connection to receive error notifications while
 *  data is not able to pass through successfully.
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
class MNcmMultiplexerConnectionObserver
    {

public:


    /**
     * Called by connection when transfer error occurs on it
     *
     * @since   S60 v3.2
     * @param   aSessionId          The ID identifying session
     * @param   aStreamId           The ID identifying stream
     * @param   aConnectionId       The ID identifying connection
     * @param   aNotifyType         Type of notify
     * @param   aError              The error code
     * @return void
     */
    virtual void ConnectionError( TUint aSessionId, TUint aStreamId,
        TUint aConnectionId,
        TMultiplexerConnectionNotifyType aNotifyType, TInt aError ) = 0;
    
    /**
     * Called by socket destination object when action completes
     * (or error is reached)
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aConnectionId       The ID identifying connection
     * @param   aType               Type of notify
     * @param   aError              Error code
     * @return  void
     */
    virtual void ConnectionNotify( TUint aStreamId, TUint aConnectionId,
        TMultiplexerConnectionNotifyType aType, TInt aError ) = 0;
    
    /**
     * Called by socket destination object when current local port is already in use.
     * Get new free port.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aConnectionId       The ID identifying connection     
     * @param   aPort               New free port
     * @return  void
     */    
    virtual void GetNewFreePort( TUint aStreamId, TUint aConnectionId,
        TUint& aPort ) = 0;

    };


#endif // M_MNCMMULTIPLEXERCONNECTIONOBSERVER_H
