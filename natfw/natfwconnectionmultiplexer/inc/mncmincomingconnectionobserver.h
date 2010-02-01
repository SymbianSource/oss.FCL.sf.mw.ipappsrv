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
* Description:    Observer interface used to receive notifications about
*                incoming connections from Connection Multiplexer
*
*/




#ifndef M_MNCMINCOMINGCONNECTIONOBSERVER_H
#define M_MNCMINCOMINGCONNECTIONOBSERVER_H

#include <e32base.h>


/**
 *  Observer interface used to receive notifications about incoming
 *  connections from Connection Multiplexer
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
class MNcmIncomingConnectionObserver
    {

public:

    /**
     * Called by Connection Multiplexer when incoming message is found.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aMessage            Message is a reference to constant message
     * @param   aConsumed           Flag to indicate is this message consumed
     * @param   aLocalAddr          A stream local address.
     * @param   aFromAddr           A remote source address. ( From socket )
     * @param   aPeerRemoteAddr     A peer remote address.
     * @return  void
     */
    virtual void IncomingMessageL( TUint aStreamId, const TDesC8& aMessage,
        const TInetAddr& aLocalAddr, const TInetAddr& aFromAddr,
        const TInetAddr& aPeerRemoteAddr, TBool& aConsumed ) = 0;

    };


#endif // M_MNCMINCOMINGCONNECTIONOBSERVER_H
