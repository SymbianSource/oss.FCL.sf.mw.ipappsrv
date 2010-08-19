/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    RTP Keepalive mechanism
*
*/




// INCLUDE FILES
#include <e32math.h>
#include "mccrtpkeepalivecontainer.h"
#include "mccrtpkeepalive.h"
#include "mccdef.h"
#include "mccinternaldef.h"
#include "mccsubcontrollerlogs.h"
#include "mccinternalevents.h"
#include "mccrtpinterface.h"


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS


// ============================= LOCAL FUNCTIONS =============================


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CMccRtpKeepaliveContainer::NewL
// ---------------------------------------------------------------------------
//
CMccRtpKeepaliveContainer* CMccRtpKeepaliveContainer::NewL( 
    MAsyncEventHandler& aEventHandler,
    CRtpAPI& aRtpAPI,
    TRtpId aRtpSessionId )
    {
    CMccRtpKeepaliveContainer* self = 
        new ( ELeave ) CMccRtpKeepaliveContainer( aEventHandler,
                                                  aRtpAPI,
                                                  aRtpSessionId );
                                                  
    return self;  
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepaliveContainer::~CMccRtpKeepaliveContainer
// ---------------------------------------------------------------------------
//        
CMccRtpKeepaliveContainer::~CMccRtpKeepaliveContainer()
    {
    iKeepaliveHandlers.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepaliveContainer::StartKeepaliveL
// ---------------------------------------------------------------------------
//
void CMccRtpKeepaliveContainer::StartKeepaliveL( 
    MMccRtpInterface& aUser, 
    TMccCodecInfo& aCodecInfo,
    CMccRtpMediaClock& aRtpMediaClock )
    {
    __SUBCONTROLLER( "CMccRtpKeepaliveContainer::StartKeepaliveL(), Entry" )
    
    if ( aUser.IsSink() )
        {
        iRtpSinkExist = ETrue;
        }  
            
    if ( ( aCodecInfo.iKeepaliveInterval > 0 && !iStopped ) && 
            ( aUser.IsSink() ||
            ( !aUser.IsSink() && EFalse == iRtpSinkExist ) ) )
        {
        __SUBCONTROLLER( "CMccRtpKeepaliveContainer::StartKeepaliveL(),\
        interval defined" )

        CMccRtpKeepalive* handler = FindKeepaliveHandler( 
            aCodecInfo.iKeepalivePT, aUser );
        if ( !handler )
            {
            handler = CMccRtpKeepalive::NewL( iEventHandler, 
                                              iRtpAPI, 
                                              iRtpSessionId, 
                                              aCodecInfo.iKeepalivePT,
                                              aCodecInfo.iKeepaliveInterval,
                                              aCodecInfo.iKeepaliveData,
                                              iRemoteAddressSet );
            CleanupStack::PushL( handler );
            iKeepaliveHandlers.AppendL( handler ); 
            CleanupStack::Pop( handler );
            }
        handler->StartKeepaliveL( aUser, aRtpMediaClock );
        }
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepaliveContainer::StopKeepalive
// ---------------------------------------------------------------------------
//        
TInt CMccRtpKeepaliveContainer::StopKeepalive( 
    MMccRtpInterface& aUser, 
    const TMccCodecInfo& aCodecInfo )
    {
    __SUBCONTROLLER( "CMccRtpKeepaliveContainer::StopKeepalive(), Entry" )
    TInt err( KErrNotFound );
    CMccRtpKeepalive* handler = FindKeepaliveHandler(
        aCodecInfo.iKeepalivePT, aUser );
    if ( handler )
        {
        TInt refCount( 0 );
        err = handler->StopKeepalive( aUser, refCount );
        if ( !refCount )
            {
            // No more users, can be removed
            TInt index = iKeepaliveHandlers.Find( handler );
            if ( index != KErrNotFound )
                {
                __SUBCONTROLLER( "CMccRtpKeepaliveContainer::StopKeepalive(),\
                delete Handler" )
                delete iKeepaliveHandlers[ index ];
                iKeepaliveHandlers.Remove( index );
                }
            }
        if ( iRtpSinkExist )
            {
            iRtpSinkExist = EFalse;            
            TInt count = iKeepaliveHandlers.Count();
            for ( TInt i = 0; i < count && !iRtpSinkExist; i++ )
                {
                iRtpSinkExist = iKeepaliveHandlers[ i ]->IsForSink();
                }
            }    
        }
    return err;
    }
    
// ---------------------------------------------------------------------------
// CMccRtpKeepaliveContainer::ResetKeepAliveTimer
// ---------------------------------------------------------------------------
//    
void CMccRtpKeepaliveContainer::ResetKeepaliveTimer()
    {
    TInt count = iKeepaliveHandlers.Count();        
    for ( TInt i = 0; i < count; i++ )
        {
        iKeepaliveHandlers[ i ]->ResetKeepaliveTimer();
        }        
    }    

// ---------------------------------------------------------------------------
// CMccRtpKeepaliveContainer::UpdateParamsL
// ---------------------------------------------------------------------------
//  
void CMccRtpKeepaliveContainer::UpdateParamsL( MMccRtpInterface& aUser,
    const TMccCodecInfo& aCodecInfo, CMccRtpMediaClock& aRtpMediaClock )
    {
    __SUBCONTROLLER( "CMccRtpKeepaliveContainer::UpdateParamsL(), Entry" )
    
    CMccRtpKeepalive* handler = 
        FindKeepaliveHandler( aCodecInfo.iKeepalivePT, aUser );
    
    if ( handler )
        {
        // Deactivate handler because update invalidates references to the
        // previous keep-alive data.
        handler->Cancel();
        handler->UpdateParamsL( aCodecInfo.iKeepalivePT, 
            aCodecInfo.iKeepaliveInterval, aCodecInfo.iKeepaliveData, &aRtpMediaClock );
        
        if ( aCodecInfo.iKeepaliveInterval > 0 && !handler->IsActive() )
            {
            handler->ResetKeepaliveTimer();
            }
        }
    else
    	{
    	TMccCodecInfo cInfo = aCodecInfo;
		StartKeepaliveL( aUser, cInfo, aRtpMediaClock );
    	}
    
    __SUBCONTROLLER( "CMccRtpKeepaliveContainer::UpdateParamsL(), exit" )
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepaliveContainer::StopAll
// ---------------------------------------------------------------------------
//    
void CMccRtpKeepaliveContainer::StopAll()
    {
    iKeepaliveHandlers.ResetAndDestroy();
    iStopped = ETrue;
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepaliveContainer::RemoteAddressSet
// ---------------------------------------------------------------------------
//   
TInt CMccRtpKeepaliveContainer::RemoteAddressSet()
    {
    TInt err( KErrNotReady );
    iRemoteAddressSet = ETrue;
    if ( !iStopped )
        {
        err = KErrNone;
        for ( TInt i = 0; i < iKeepaliveHandlers.Count(); i++ )
            {
            err |= iKeepaliveHandlers[ i ]->RemoteAddressSet();
            }
        }
    return err;
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepaliveContainer::FindKeepaliveHandler
// ---------------------------------------------------------------------------
//   
CMccRtpKeepalive* CMccRtpKeepaliveContainer::FindKeepaliveHandler( 
    TUint8 aKeepAlivePT, MMccRtpInterface& aUser )
    {
    CMccRtpKeepalive* handler = NULL;
    for ( TInt i = 0; i < iKeepaliveHandlers.Count() && !handler; i++ )
        {
        if ( aUser.IsSink() )
            {
            if ( iKeepaliveHandlers[ i ]->StreamId() == aUser.RtpStreamId() )
                {
                handler = iKeepaliveHandlers[ i ];
                }
            }
        else
            {
            if ( iKeepaliveHandlers[ i ]->PayloadType() == aKeepAlivePT )
                {
                handler = iKeepaliveHandlers[ i ];
                }
            }
        }
    return handler;
    }
    
// ---------------------------------------------------------------------------
// CMccRtpKeepaliveContainer::CMccRtpKeepaliveContainer
// ---------------------------------------------------------------------------
//        
CMccRtpKeepaliveContainer::CMccRtpKeepaliveContainer( 
    MAsyncEventHandler& aEventHandler,
    CRtpAPI& aRtpAPI,
    TRtpId aRtpSessionId ) :
    iEventHandler( aEventHandler ), 
    iRtpAPI( aRtpAPI ),
    iRtpSessionId( aRtpSessionId )
    {
    }      

       
// ========================== OTHER EXPORTED FUNCTIONS =======================

//  End of File  
