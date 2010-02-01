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
* Description:  Interface for attaching socket-based medias into 
                 NAT framework.
*
*/


#ifndef M_SOCKETMEDIACONNWRAPPER_H
#define M_SOCKETMEDIACONNWRAPPER_H

#include <e32def.h>
#include "natfwconnectivityapidefs.h"

class TSockAddr;
class MNsmcwTcpConnectionObserver;

/**
 *  Interface for attaching socket-based medias into NATFW. Provides
 *  services to control datapath between mediaengine and NATFW.
 *  
 *  @lib socketmediaconnwrapper.dll
 *  @since S60 v3.2
 */
class MNATFWSocketMediaConnWrapper
    {

public:

    /**
     * Returns local proxy address through which client can communicate
     * with its peer. All data sent to this address will be forwarded
     * to the peer if sending is fully activated through wrapper and
     * NAT Connectivity APIs. Returned address is also used to forward
     * incoming data from the peer.
     * 
     * @since   S60 v3.2
     * @param   aAddr       Address for sending to peer
     */
    virtual void OutgoingAddr( TSockAddr& aAddr ) = 0;
    
    /**
     * Sets address where NATFW will forward incoming data from the peer
     * when receiving is activated.
     *
     * @since   S60 v3.2
     * @param   aAddr       Address for receiving from peer
     */
    virtual void SetIncomingAddrL( const TSockAddr& aAddr ) = 0;
    
    /**
     * Returns identifier of NAT-stream that wrapper is associated with.
     *
     * @since   S60 v3.2
     * @return  Stream identifier
     */
    virtual TUint StreamId() = 0;
    
    
    /**
     * Enables/disables datapath between mediaengine and NATFW regarding
     * media receiving.
     * 
     * @since S60 v3.2
     * @param   aState          The receiving state
     */
    virtual void SetReceivingStateL( TNATFWStreamingState aState ) = 0;
    
    /**
     * Enables/disables datapath between mediaengine and NATFW regarding
     * media sending.
     * 
     * @since S60 v3.2
     * @param   aState          The sending state
     */
    virtual void SetSendingStateL( TNATFWStreamingState aState ) = 0;
    };

#endif // M_SOCKETMEDIACONNWRAPPER_H
