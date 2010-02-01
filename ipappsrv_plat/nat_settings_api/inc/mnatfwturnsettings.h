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
* Description:    Interface for querying TURN related software settings.
*
*/





#ifndef M_MNATFWTURNSETTINGS_H
#define M_MNATFWTURNSETTINGS_H


#include <e32base.h>


class MNATFWServerSettings;


/**
 *  Interface for querying TURN related software settings.
 *  
 *  Interface is not intended for derivation outside NATFW.
 *  
 *  @lib natfwnatsettings.dll
 *  @since S60 v3.2
 */
class MNATFWTurnSettings
    {

public:

    /**
     * Returns array of TURN server related settings.
     * Ownership is not transferred.
     *
     * @since S60 3.2
     * @param aServers Array of server pointers
     */
    virtual void GetTurnServerArrayL(
        RPointerArray<MNATFWServerSettings>& aServers ) const = 0;
    
    /**
     * Returns initial retransmission timeout (RTO).
     *
     * @since   S60 v3.2
     * @return  Retransmission timeout
     */  
    virtual TUint RetransmissionTimeout() const = 0;
       
    /**
     * Returns latest successfully connected TURN server address.
     *
     * @since S60 3.2
     * @return Address
     */
    virtual const TDesC8& LatestConnectedServerAddr() const = 0;
    
    /**
     * Returns latest successfully connected TURN server port.
     *
     * @since S60 3.2
     * @return Port number
     */
    virtual TInt LatestConnectedServerPort() const = 0;

    };


#endif // M_MNATFWSTUNSETTINGS_H
