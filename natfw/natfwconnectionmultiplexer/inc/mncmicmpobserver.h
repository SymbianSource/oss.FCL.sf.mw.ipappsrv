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
* Description:    Observer interface used to receive notifications from
*                Connection Multiplexer ICMP receiver.
*
*/




#ifndef M_MNCMICMPOBSERVER_H
#define M_MNCMICMPOBSERVER_H

// INCLUDES
#include <e32base.h>

class TInetAddr;

/**
 *  Observer interface used to receive notifications from
 *  Connection Multiplexer ICMP receiver
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
class MNcmIcmpObserver
    {
    public:          

        /**
         * Notifies that an ICMP error is occurred using
         * the specified remote address. Gets a local address and
         * a remote address to be used in the creation of the ICMP message.
         *
         * @since   S60 v3.2
         * @param   aAddress        Remote address on which the error occurred
         * @param   aLocalAddress   Reference to local address
         * @param   aRemoteAddress  Reference to remote address
         * @return  void
         */
        virtual void IcmpError( const TInetAddr& aAddress,
            TInetAddr& aLocalAddress, TInetAddr& aRemoteAddress ) = 0;
    };

#endif  // M_MNCMICMPOBSERVER_H  
           
//  End of File