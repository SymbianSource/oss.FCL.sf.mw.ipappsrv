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
* Description:    Observer class that the client must implement to receive notifications about
*                incoming and outgoing messages. Allows observer to observe and 
*                make modifications to offered messages.
*
*/




#ifndef M_MNCMMESSAGEOBSERVER_H
#define M_MNCMMESSAGEOBSERVER_H

#include <e32base.h>


/**
 *  Observer class that the client must implement to receive notifications about
 *  incoming and outgoing messages. Allows observer to observe and 
 *  make modifications to offered messages
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
class MNcmMessageObserver
    {

public:

    /**
     * Called by Connection Multiplexer when incoming message is found.
     * Allows connection observer to observe and make modifications to found
     * message by offering an own copy of content with modification(s).
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aMessage            Message is a reference to constant message
     * @param   aLocalAddr          A stream local address.
     * @param   aFromAddr           A remote source address. ( From socket )
     * @param   aPeerRemoteAddr     A peer remote address.
     * @return  Pointer to a heap descriptor with new content for message,
     *          otherwise NULL is returned. Ownership of the heap descriptor is
     *          transferred to connection multiplexer ( from observer ).
     */
    virtual HBufC8* IncomingMessageNotify( TUint aStreamId,
        const TDesC8& aMessage, const TInetAddr& aLocalAddr,
        const TInetAddr& aFromAddr, TInetAddr& aPeerRemoteAddr ) = 0;


    /**
     * Called by Connection Multiplexer when outgoing message is found.
     * Allows connection observer to observe and make modifications to found
     * message by offering an own copy of content with modification(s).
     *
     * @since S60 v3.2
     * @param aStreamId             The ID identifying stream
     * @param aConnectionId         The ID identifying connection
     * @param aDestinationAddress   Destination address where the packet
     *                              is being sent to.   
     * @param aMessage              Message is a reference to constant message
     *                              which was found
     * @return pointer to a heap descriptor with new content for message,
     *         otherwise NULL is returned. Ownership of the heap descriptor is
     *         transferred to multiplexer (from observer).
     */
    virtual HBufC8* OutgoingMessageNotify( TUint aStreamId,
        TUint aConnectionId, const TInetAddr& aDestinationAddress,
        const TDesC8& aMessage ) = 0;      
    };


#endif // M_MNCMMESSAGEOBSERVER_H
