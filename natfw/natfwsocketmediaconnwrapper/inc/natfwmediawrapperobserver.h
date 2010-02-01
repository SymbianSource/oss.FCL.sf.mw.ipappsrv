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




#ifndef M_MEDIA_WRAPPER_OBSERVER_H
#define M_MEDIA_WRAPPER_OBSERVER_H

#include <e32base.h>

/**
 *  Observer interface used to reveice notifications about incoming
 *  connections from sender
 *
 *  Defines source for multiplexer, where Media Wrapper is mounted to
 *
 *  @lib socketmediaconnwrapper.dll
 *  @since S60 v3.2
 */
class MNATFWMediaWrapperObserver
    {

public:

    /**
     * Indicates how sending to user succeed
     *
     * @since S60 v3.2
     * @param aError system-wide error code
     * @return void
     */
    virtual void SendingCompleted( TInt aError ) = 0;
    
    /**
     * Called after data were sent to wrapper. Delivers it to multiplexer.
     *
     * @since S60 v3.2
     * @param aStreamId an ID for stream the data was received from
     * @param aStreamPortion stream portion which were sent by client
     * @return system-wide error code
     */
    virtual TInt ReceivedFrom( TUint aStreamId, TDes8& aStreamPortion  ) = 0;

    /**
     * Called by socket media wrapper object when local port is already in use.
     * Get new free port.
     *
     * @since   S60 v3.2
     * @param   aPort               New free port
     * @return  void
     */    
    virtual void GetNewFreePort( TUint& aPort ) = 0;
    
    };


#endif // M_MEDIA_WRAPPER_OBSERVER_H
