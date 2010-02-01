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
* Description:    Observer for socket receiver and sender
*
*/




#ifndef M_MNCMRECEIVEROBSERVER_H
#define M_MNCMRECEIVEROBSERVER_H

#include <e32base.h>


/**
 *  Observer for socket receiver/sender
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
class MNcmReceiverSenderObserver
    {

public:

    /**
     * Called by socket receiver while message is found
     *
     * @since   S60 v3.2
     * @return  void
     */
    virtual void MessageAvailable() = 0;

    /**
     * Called by socket receiver/sender while error occurs
     *
     * @param   aError              Error code
     * @return  void
     */
    virtual void Error( TInt aError ) = 0;
    };


#endif // M_MNCMRECEIVEROBSERVER_H
