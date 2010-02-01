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
* Description:    Observer interface used to receive notifications from
*                Connection Multiplexer.
*
*/




#ifndef M_MNCMSOURCEOBSERVER_H
#define M_MNCMSOURCEOBSERVER_H

#include <e32base.h>

class MNcmSenderObserver;

/**
 *  Observer interface used to notify multiplexer from new outgoing message
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
class MNcmSourceObserver
    {

public:

    /**
     * Called by source object when a new message is found
     *
     * @since S60 v3.2
     * @param aMessage          The outgoing message
     * @param aSenderObserver   Observer for this send
     * @param aDestAddr         Destination address
     */
    virtual void OutgoingMessageAvailable( const TDesC8& aMessage,
        MNcmSenderObserver* aSenderObserver, const TInetAddr& aDestAddr ) = 0;

    };


#endif // M_MNCMSOURCEOBSERVER_H
