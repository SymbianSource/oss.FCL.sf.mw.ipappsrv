/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Observer
*
*/




#ifndef M_SOCKET_SENDER_OBSERVER_H
#define M_SOCKET_SENDER_OBSERVER_H

#include <e32def.h>

/**
 *  Observer
 *
 *  Indicates actions to SocketMediaConnWrapper
 *
 *  @lib socketmediaconnwrapper.dll
 *  @since S60 v3.2
 */
class MNATFWSocketSenderObserver
    {

public:
    
    /**
     * Indicates when sender is completed.
     *
     * @since S60 v3.2
     * @param aError system-wide error code
     * @return void
     */
    virtual void SendingCompleted( TInt aError ) = 0;
     
    };


#endif // M_SOCKET_SENDER_OBSERVER_H
