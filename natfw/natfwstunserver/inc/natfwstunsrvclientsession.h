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
* Description:    Provides client interface of the NAT FW STUN server
*
*/




#ifndef NATFWSTUNSRVCLIENTSESSION_H
#define NATFWSTUNSRVCLIENTSESSION_H

#include <e32base.h>
#include "natfwconnectivityapidefs.h"

class CNATFWSTUNSrvImpl;
class MNATFWStunSrvObserver;
class CNATFWCredentials;
class MNcmConnectionMultiplexer;
class TInetAddr;

/**
 * Interface of the NATFW STUN server, which client uses to listen to 
 * incoming STUN requests. Also Turn spesific data indications are decoded.
 *
 *  @lib stunserver.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNATFWSTUNSrvClientSession ): public CBase
    {

public:
    
    /**
     * Two-phased constructor.
     * @param aObserver    A callback object to receive and handle
     *                     event notifications from NAT FW STUN server.
     * @param aMultiplexer multiplexer.
     */
    IMPORT_C static CNATFWSTUNSrvClientSession* NewL( 
                                MNATFWStunSrvObserver& aObserver,
                                MNcmConnectionMultiplexer& aMultiplexer );
    /**
     * Destructor.
     */
    virtual ~CNATFWSTUNSrvClientSession();

    /**
     * Starts listening to incoming STUN requests.
     * Notifies the client of STUN requests that are valid.
     *  
     * @since S60 3.2
     * @param aIdentifications Authentication passwords
     * @return void
     */
    IMPORT_C void AddAuthenticationParamsL( 
        const RPointerArray<CNATFWCredentials>& aIdentifications );
     
    /**
     * Stops listening to incoming STUN requests.
     *  
     * @since S60 3.2
     * @param aIdentifications Authentication passwords
     * @return void
     */
    IMPORT_C void RemoveAuthenticationParamsL( 
        const RPointerArray<CNATFWCredentials>& aIdentifications );
        
    /**
     * Sets role for an agent. 
     *  
     * @since S60 3.2
     * @param aRole       Either controlling or controlled.  
     * @param aTieBreaker Value for determining which agent will change
     *                    Role if role conflict is present
     * @return void
     */        
    IMPORT_C void SetRoleL( TNATFWIceRole aRole, 
        TUint64 aTieBreaker );         
                       

private:

    CNATFWSTUNSrvClientSession();
    
    void ConstructL( MNATFWStunSrvObserver& aObserver,
                     MNcmConnectionMultiplexer& aMultiplexer );
    
private:

    /*
     * NAT FW STUN server implementation
     * Own.
     */ 
    CNATFWSTUNSrvImpl* iImplementation;
    };

#endif // NATFWSTUNSRVCLIENTSESSION_H
