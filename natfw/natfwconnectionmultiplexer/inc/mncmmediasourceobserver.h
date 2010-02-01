/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Observer for media source(s)
*
*/




#ifndef M_MNCMMEDIASOURCEOBSERVER_H
#define M_MNCMMEDIASOURCEOBSERVER_H

#include <e32base.h>

/**
 *  Observer for media source(s)
 *
 *  Used by CNcmConnectionMultiplexer for receiving wrapper sending
 *  error notifies
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
class MNcmMediaSourceObserver
    {

public:

    /**
     * Called by Media Source after wrapper has had sending error
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream   
     * @param   aError              The error code
     * @return  void
     */ 
    virtual void WrapperSenderError( TUint aStreamId, TInt aError ) = 0;

    /**
     * Called by Media Source if current local port is already in use.
     * Gets new free port.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream   
     * @param   aPort               New free port
     * @return  void
     */     
    virtual void GetFreePort( TUint aStreamId, TUint& aPort ) = 0;
    
    /**
     * Notifies that the first media packet is sent to
     * the remote end.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @return  void
     */     
    virtual void FirstMediaPacketSent( TUint aStreamId ) = 0;
    };


#endif // M_MNCMMEDIASOURCEOBSERVER_H
