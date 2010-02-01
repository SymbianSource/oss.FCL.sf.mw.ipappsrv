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
* Description:    Interface for connecting media-engines into nat framework.
*
*/




#ifndef M_MEDIA_WRAPPER_H
#define M_MEDIA_WRAPPER_H

#include <e32base.h>
#include "natfwconnectivityapidefs.h"

#include "natfwmediawrapperobserver.h"

class TInetAddr;
class RSocketServ;

/**
 *  Media Wrapper Interface
 *
 *  Interface for connecting different media-engines into nat framework.
 *
 *  @lib socketmediaconnwrapper.dll
 *  @since S60 v3.2
 */
class MNATFWMediaWrapper
    {

public:
    
    /**
     * Enables wrapper to forward packets from mediaengine to connection
     * multiplexer.
     * 
     * @since S60 v3.2
     * @param   aState          The sending state
     * @return void
     */     
    virtual void SetReceivingStateForMuxWrapper(
        TNATFWStreamingState aState ) = 0;             
        
    /**
     * Send data to connected media engine.
     *
     * @since S60 v3.2
     * @param aStreamPortion a reference to buffer to be sent
     * @return void
     */
    virtual void SendL( const TDesC8& aStreamPortion ) = 0; 
    
    /**
     * Returns The stream ID which wrapper is associated with
     *
     * @since S60 v3.2
     * @return ID which wrapper is associated with
     */
    virtual TUint StreamId() = 0;
    
    /**
     * Register media observer.
     *
     * @since S60 v3.2
     * @param aObserver  media observer 
     * @param aServer    socket server handle
     * @param aLocalAddr wrapper's local address
     * @return void   
     */
    virtual void RegisterMediaWrapperObserverL(
        MNATFWMediaWrapperObserver* aObserver,
        const RSocketServ& aServer,
        const TInetAddr& aLocalAddr ) = 0;

    /**
     * Closes wrapper instance by deleting it.
     *
     * Implementing class MUST call its destructor on this method and free
     * allocated heap. Usually this is done by "delete this".
     *
     * @since S60 v3.2
     * @return void
     */    
    virtual void Close() = 0;

    /**
     * Gets wrapper's local and remote addresses
     *
     * @since S60 v3.2
     * @param aLocalAddr    wrapper's local address 
     * @param aRemoteAddr   wrapper's remote address
     * @return void   
     */
    virtual void GetAddresses(
        TInetAddr& aLocalAddr,
        TInetAddr& aRemoteAddr ) = 0;
    };


#endif // M_MEDIA_WRAPPER_H
