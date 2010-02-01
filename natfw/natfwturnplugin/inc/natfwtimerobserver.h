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
* Description:    Observer class.
*
*/




#ifndef C_NATFWTIMEROBSERVER_H
#define C_NATFWTIMEROBSERVER_H

#include <e32base.h>

/**
 *  Observer class for timing of set Active Destination request process
 *
 *  @lib turnplugin.lib
 *  @since S60 v3.2
 */
class MNATFWTimerObserver
    {
    
public:

    /**
     * Notifies when Active Destination Timer has triggered.
     *
     * @since S60 v3.2
     * @param  aStreamId        To identify the binding that the
     *                          triggering was related to
     * @param  aConnectionId    To identify the binding that the
     *                          triggering was related to
     */
    virtual void TimerTriggeredL( TUint aStreamId, TUint aConnectionId ) = 0;
    
    };

#endif // C_NATFWTIMEROBSERVER_H