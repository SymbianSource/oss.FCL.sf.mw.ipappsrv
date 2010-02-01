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
*                outgoing connections from Connection Multiplexer
*
*/




#ifndef M_MNCMOUTGOINGCONNECTIONOBSERVER_H
#define M_MNCMOUTGOINGCONNECTIONOBSERVER_H

#include <e32base.h>

/**
 *  Observer interface used to receive notifications about outgoing
 *  connections from Connection Multiplexer
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
class MNcmOutgoingConnectionObserver
    {

public:

    /**
     * Called by Connection Multiplexer when outgoing message is found.
     *
     * @since S60 v3.2
     * @param aStreamId             Stream where message was found
     * @param aConnectionId         Connection where message was found
     * @param aDestinationAddress   Destination address where the packet
     *                              is being sent to. If this is set then
     *                              connection ID is ignored       
     * @param aMessage              Message is a reference to constant message
     *                              which was found
     * @param aConsumed             Is reference to a boolean variable which
     *                              MUST be set by observer: ETrue is message
     *                              was consumed, EFalse otherwise.
     * @return void
     */
    virtual void OutgoingMessageL( TUint aStreamId, TUint aConnectionId,
        const TInetAddr& aDestinationAddress, const TDesC8& aMessage,
        TBool& aConsumed ) = 0;

    };


#endif // M_MNCMOUTGOINGCONNECTIONOBSERVER_H
