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

#ifndef CNSPWRAPPER_H
#define CNSPWRAPPER_H

#include <in_sock.h>
#include "mnatfwsocketmediaconnwrapper.h"
#include "nsputdefinitions.h"


class CNSPWrapper : public MNATFWSocketMediaConnWrapper
    {
public:

    static CNSPWrapper* NewL( TUint aStreamId );
    virtual ~CNSPWrapper();

    
private:
    
    CNSPWrapper( TUint aStreamId );
    void ConstructL();


public: // From MNATFWSocketMediaConnWrapper

    /**
     * User of NAT Connectivity Framework fetch the reserved local IP which
     * is used as destination address
     * 
     * @since S60 v3.2
     * @param aSocketToSendForPeer Address for receiving data
     * @return void
     */
    void OutgoingAddr( TSockAddr& aSocketToSendForPeer );
    
    /**
     * User of NAT Connectivity Framework sets local socket address for incoming packets
     *
     * @since S60 v3.2
     * @param aSocketToReceiveFromPeer Destination address for sending
     * @return void
     */
    void SetIncomingAddrL( const TSockAddr& aSocketToReceiveFromPeer );
    
    /**
     * Returns The stream ID which wrapper is associated with
     *
     * @since S60 v3.2
     * @return ID which wrapper is associated with
     */
    TUint StreamId();
    
    /**
     * Enables/disables datapath between mediaengine and NATFW regarding
     * media receiving.
     * 
     * @since S60 v3.2
     * @param   aState          The receiving state
     */
    void SetReceivingStateL( TNATFWStreamingState aState );
    
    /**
     * Enables/disables datapath between mediaengine and NATFW regarding
     * media sending.
     * 
     * @since S60 v3.2
     * @param   aState          The sending state
     */
    void SetSendingStateL( TNATFWStreamingState aState );    

private: // data

    TInetAddr iOutgoingAddress;
    TInetAddr iIncomingAddress;
    
    NSP_UT_DEFINITIONS
    
    };

    
#endif // CNSPWRAPPER_H
