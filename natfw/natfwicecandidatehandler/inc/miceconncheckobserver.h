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
* Description:    
*
*/




#ifndef M_MICECONNCHECKOBSERVER_H
#define M_MICECONNCHECKOBSERVER_H

#include <e32base.h>

class TInetAddr;

class MIceConnCheckObserver
    {
public:
    /**
     * Connectivity check has completed.
     *
     * @since   S60 3.2
     * @param   aCompletionCode System wide error code indicating success/fail
     * @param   aLocalAddr      The local address where the response was sent to
     * @param   aPeerAddr       The peer address where the response was sent from
     * @param   aMappedAddr     Mapped address from response
     */
    virtual void ConnCheckCompletedL(
        TInt aCompletionCode,
        const TInetAddr& aLocalAddr,
        const TInetAddr& aPeerAddr,
        const TInetAddr& aMappedAddr ) = 0;
    };

#endif // M_MICECONNCHECKOBSERVER_H