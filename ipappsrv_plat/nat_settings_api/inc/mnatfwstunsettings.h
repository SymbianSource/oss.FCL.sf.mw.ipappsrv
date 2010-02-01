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
* Description:    Interface for querying STUN related software settings.
*
*/





#ifndef M_MNATFWSTUNSETTINGS_H
#define M_MNATFWSTUNSETTINGS_H


#include <e32base.h>


class MNATFWServerSettings;


/**
 *  Interface for querying STUN related software settings.
 *  
 *  Interface is not intended for derivation outside NATFW.
 *  
 *  @lib natfwnatsettings.dll
 *  @since S60 v3.2
 */
class MNATFWStunSettings
    {

public:

    /**
     * Returns array of STUN server related settings.
     * Ownership is not transferred.
     *
     * @since S60 3.2
     * @param aServers Server settings pointers
     */
    virtual void GetStunServerArrayL(
        RPointerArray<MNATFWServerSettings>& aServers ) const = 0;
   
    /**
     * Returns initial retransmission timeout (RTO).
     *
     * @since S60 v3.2
     * @return Retransmission timeout
     */  
    virtual TUint RetransmissionTimeout() const = 0;
    
    /**
     * Returns latest successfully connected STUN server address.
     *
     * @since S60 3.2
     * @return address
     */
    virtual const TDesC8& LatestConnectedServerAddr() const = 0;
    
    /**
     * Returns latest successfully connected STUN server port.
     *
     * @since S60 3.2
     * @return port number
    */
    virtual TInt LatestConnectedServerPort() const = 0;

    };


#endif // M_MNATFWSTUNSETTINGS_H
