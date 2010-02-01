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
* Description:    Observer interface used to receive notifications from
*                Connection Multiplexer session.
*
*/




#ifndef M_MNCMSESSIONOBSERVER_H
#define M_MNCMSESSIONOBSERVER_H

// INCLUDES
#include <e32base.h>

class TInetAddr;

/**
 *  Observer interface used to receive notifications from
 *  Connection Multiplexer session
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
class MNcmSessionObserver
    {
    public:          

        /**
         * Notifies that session creation is failed.
         *
         * @since   S60 v3.2
         * @param   aSessionId      The ID identifying session
         * @return  void
         */
        virtual void SessionCreationFailed( TUint aSessionId ) = 0;
    };

#endif  // M_MNCMSESSIONOBSERVER_H  
           
//  End of File