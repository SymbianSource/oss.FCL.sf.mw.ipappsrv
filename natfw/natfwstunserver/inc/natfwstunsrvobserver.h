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
* Description:    Observer interface used to receive notifications about
*                incoming STUN requests from NAT FW STUN server
*
*/




#ifndef NATFWSTUNSRVOBSERVER_H
#define NATFWSTUNSRVOBSERVER_H

#include "natfwconnectivityapidefs.h"

/**
 * Observer interface used to receive notifications about
 * incoming STUN requests from NAT FW STUN server
 *
 * @lib stunserver.lib
 * @since S60 v3.2
 */
class MNATFWStunSrvObserver
    {

public:

    /**
     * Called by NATFW STUN Server when incoming STUN request is found.
     *
     * @since S60 v3.2
     * @param aLocalAddr     local address. 
     * @param aFromAddr      address the server saw in request.
     * @param aPeerAddr      peer remote address.
     * @param aPriority      proirity.
     * @param aRemoteFavored Candidate pair resulting from this check should 
     *                       be used for transmission of media if true.
     * @return void
     */
    virtual void STUNRequestReceivedL( const TInetAddr& aLocalAddr,
        const TInetAddr& aFromAddr, const TInetAddr& aPeerAddr,
        TUint aPriority, TBool aRemoteFavored ) = 0;
        
        
    /**
     * Called by NATFW Stun Server if role conflict is present between agents
     * and role change is needed to this agent. 
     *
     * @since S60 v3.2
     * @param aDesiredRole Desired role.    
     * @return void
     */
    virtual void RoleChangeNeeded( TNATFWIceRole aDesiredRole ) = 0;  

    };


#endif // NATFWSTUNSRVOBSERVER_H
