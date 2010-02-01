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
* Description:    Observer
*
*/




#ifndef M_NSMCWTCPCONNECTIONOBSERVER_H
#define M_NSMCWTCPCONNECTIONOBSERVER_H

#include <e32def.h>

/**
 *  TCP connection observer
 *
 *  Observer interface for notifying client of a SetupTcpConnectionL request 
 *  completion.
 *
 *  @lib socketmediaconnwrapper.dll
 *  @since S60 v3.2
 */
class MNsmcwTcpConnectionObserver
    {

public:

    /**
     * Indicates how connecting succeeds
     *
     * @since S60 v3.2
     * @param aError system-wide error code
     * @return void
     */
    virtual void ConnectingCompleted( TInt aError ) = 0;

    };


#endif // M_NSMCWTCPCONNECTIONOBSERVER_H
