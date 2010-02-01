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
* Description:    Defines connection handler observer interface.
*
*/




#ifndef M_ICECONNHANDLEROBSERVER_H
#define M_ICECONNHANDLEROBSERVER_H

#include <e32base.h>

/**
 *  Defines observer interface for connection handler user.
 *
 *  @lib icecandidatehandler.dll
 *  @since S60 v3.2
 */
class MIceConnHandlerObserver
    {

public:

    /**
     * Notifies that stream has been closed.
     * 
     * @since   S60 3.2
     * @param   aStreamId       The stream identifier
     */
    virtual void StreamClosed( TUint aStreamId ) = 0;

    };


#endif // M_ICECONNHANDLEROBSERVER_H
