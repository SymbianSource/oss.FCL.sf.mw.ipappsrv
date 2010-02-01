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
* Description:    Observer interface used to receive notifications about
*                messages sent to network
*
*/




#ifndef M_MNCMSENDEROBSERVER_H
#define M_MNCMSENDEROBSERVER_H

#include <e32std.h>


/**
 *  Observer interface used to receive notifications about
 *  messages sent to network.
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
class MNcmSenderObserver
    {

public:
     
    /**
     * Called by Connection Multiplexer when
     * sending the message is succesfully completed.
     *
     * @since   S60 v3.2
     * @return  void
     */ 
    virtual void MessageSent() = 0;

    /**
     * Called by Connection Multiplexer when 
     * sending the message is failed.
     *
     * @since   S60 v3.2
     * @param   aError      A reason for the failure.
     * @return  void
     */
    virtual void MessageSentFailure( TInt aError ) = 0;
    };


#endif // M_MNCMSENDEROBSERVER_H
