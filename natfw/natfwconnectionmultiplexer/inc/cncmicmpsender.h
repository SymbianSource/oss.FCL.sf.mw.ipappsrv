/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    ICMP error sender
*
*/




#ifndef C_CNCMICMPSENDER_H
#define C_CNCMICMPSENDER_H

// INCLUDES
#include <e32base.h>
#include <in_sock.h>

/**
 *  ICMP error sender
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNcmIcmpSender ) : public CActive
    {

public:

    /** 
     * Two-phased constructor
     *
     * @param   aSocket             Reference to socket
     */
    static CNcmIcmpSender* NewL( RSocket& aSocket );
        
    /**
     * Destructor
     */
    virtual ~CNcmIcmpSender();

    /**
     * Send action
     *
     * @since S60 v3.2
     * @param aMessage          Message to be send.
     * @param aDestinationAddr  Destination address
     */
    void Send( const TDesC8& aMessage, const TInetAddr& aDestinationAddr );

protected:

    CNcmIcmpSender( RSocket& aSocket );
    
// from base class CActive

    void RunL();
    
    void DoCancel();
    
protected: // data

    /**
     * ICMP socket
     */
    RSocket& iIcmpSocket;

    /**
     * Data for sending
     * Own.
     */
    HBufC8* iData;

    /**
     * Destiantion address.
     */
    TInetAddr iDestination;

    };


#endif // C_CNCMICMPSENDER_H
