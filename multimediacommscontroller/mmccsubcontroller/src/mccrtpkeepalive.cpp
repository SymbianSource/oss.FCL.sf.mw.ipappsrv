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
* Description:    RTP Keepalive mechanism
*
*/




// INCLUDE FILES
#include <e32math.h>
#include "mccrtpkeepalive.h"
#include "mccdef.h"
#include "mccinternaldef.h"
#include "mccsubcontrollerlogs.h"
#include "mccinternalevents.h"
#include "mccrtpinterface.h"
#include "mccrtpmediaclock.h"


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
// CMccRtpKeepalive::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMccRtpKeepalive* CMccRtpKeepalive::NewL( 
    MAsyncEventHandler& aEventHandler, 
    CRtpAPI& aRtpAPI,
    TRtpId aRtpSessionId,
    TUint8 aKeepalivePayloadType,
    TUint32 aKeepaliveInterval,
    const TDesC8& aKeepaliveData,
    TBool aRemoteAddressSet )
    {
    CMccRtpKeepalive* self = 
        new( ELeave ) CMccRtpKeepalive( aEventHandler, aRtpAPI, 
                                        aRtpSessionId, aRemoteAddressSet );
    CleanupStack::PushL( self );
    self->ConstructL( aKeepalivePayloadType, aKeepaliveInterval, 
                      aKeepaliveData );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepalive::~CMccRtpKeepalive
// Destructor
// ---------------------------------------------------------------------------
//
CMccRtpKeepalive::~CMccRtpKeepalive()
    {
    __SUBCONTROLLER( "CMccRtpKeepalive::~CMccRtpKeepalive()" )
    
    Cancel();
    
    if ( KNullId != iRtpStreamId && !iRtpSinkExists )
        {
        iRtpAPI.CloseStream( iRtpStreamId );
        }
    
    iTimer.Close();
    
    delete iKeepaliveData;
    
    iUsers.Reset();
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepalive::StartKeepaliveL
// ---------------------------------------------------------------------------
//
void CMccRtpKeepalive::StartKeepaliveL( MMccRtpInterface& aUser,
                                        CMccRtpMediaClock& aRtpMediaClock )
    {
    __SUBCONTROLLER( "CMccRtpKeepalive::StartKeepaliveL()" )
    
    iRtpMediaClock = &aRtpMediaClock;

    if ( KErrNotFound == iUsers.Find( &aUser ) )
        {
        iUsers.AppendL( &aUser );
        }   
                   
    User::LeaveIfError( ToggleKeepaliveSending( ResolveActivity() ) ); 
    
    __SUBCONTROLLER( "CMccRtpKeepalive::StartKeepaliveL(), exit" )
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepalive::StopKeepalive
// ---------------------------------------------------------------------------
//       
TInt CMccRtpKeepalive::StopKeepalive( MMccRtpInterface& aUser, 
                                      TInt& aRefCount )
    {
    __SUBCONTROLLER( "CMccRtpKeepalive::StopKeepalive()" )
    
    TInt err( KErrNotFound );
    TInt index = iUsers.Find( &aUser );
    if ( KErrNotFound != index)
        {
        iUsers.Remove( index );

        err = ToggleKeepaliveSending( ResolveActivity() );
        if ( err )
            {
            SendErrorToClient( err );
            }
        }
        
    __SUBCONTROLLER_INT1( "CMccRtpKeepalive::StopKeepalive(), exit with err:", err )
    
    aRefCount = iUsers.Count();
    
    return err;
    }
    
// ---------------------------------------------------------------------------
// CMccRtpKeepalive::ResetKeepaliveTimer
// ---------------------------------------------------------------------------
//       
void CMccRtpKeepalive::ResetKeepaliveTimer( )
    {
    __SUBCONTROLLER_INT1( "CMccRtpKeepalive::ResetKeepaliveTimer() active:", 
        IsActive() )
        
    Cancel();
    StartSending();
    }    

// ---------------------------------------------------------------------------
// CMccRtpKeepalive::StreamId
// ---------------------------------------------------------------------------
//    
TRtpId CMccRtpKeepalive::StreamId() const
    {
    return iRtpStreamId;
    }

// -----------------------------------------------------------------------------
// CMccRtpKeepalive::PayloadType
// -----------------------------------------------------------------------------
//    
TUint8 CMccRtpKeepalive::PayloadType() const
    {
    return iRtpHeaderInfo.iPayloadType;
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepalive::RemoteAddressSet
// ---------------------------------------------------------------------------
//    
TInt CMccRtpKeepalive::RemoteAddressSet()
    {
    __SUBCONTROLLER( "CMccRtpKeepalive::RemoteAddressSet()" )
    
    TInt err( KErrNone );
    iRemoteAddressSet = ETrue;
    if ( iCurrentState == ESendingPending )
        {
        err = StartSending();
        }
        
    __SUBCONTROLLER_INT1( "CMccRtpKeepalive::RemoteAddressSet(), exit with err:", 
        err )
    return err;
    }
    
// ---------------------------------------------------------------------------
// CMccRtpKeepalive::IsForSink
// ---------------------------------------------------------------------------
//   
TBool CMccRtpKeepalive::IsForSink()
    {
    TBool sinkFound = EFalse;
    TInt count = iUsers.Count();
    for ( TInt i = 0; i < count && !sinkFound; i++ )
        {
        sinkFound = iUsers[i]->IsSink();
        }
    return sinkFound;    
    }    

// ---------------------------------------------------------------------------
// CMccRtpKeepalive::UpdateParamsL
// ---------------------------------------------------------------------------
//   
void CMccRtpKeepalive::UpdateParamsL( TUint8 aKeepalivePT, 
    TUint32 aKeepaliveInterval, const TDesC8& aKeepaliveData, CMccRtpMediaClock* aRtpMediaClock )
    {
    __SUBCONTROLLER_INT1( 
        "CMccRtpKeepalive::UpdateParamsL(), PT:", aKeepalivePT )
    __SUBCONTROLLER_INT1( 
        "CMccRtpKeepalive::UpdateParamsL(), INT:", aKeepaliveInterval )

    __ASSERT_ALWAYS( aKeepalivePT < KMccPayloadTypeMax, 
        User::Leave( KErrArgument ) );
    
    iRtpHeaderInfo.iPayloadType = aKeepalivePT;
    
    HBufC8* tmp = aKeepaliveData.AllocL();
    delete iKeepaliveData;
    iKeepaliveData = tmp;    
    
    iKeepaliveInterval = aKeepaliveInterval;
	iRtpMediaClock = aRtpMediaClock;
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepalive::RunL
// ---------------------------------------------------------------------------
//
void CMccRtpKeepalive::RunL()
    {
    TInt status = iStatus.Int();
    
    __SUBCONTROLLER_INT1( "CMccRtpKeepalive::RunL(), status:", status )
    
    User::LeaveIfError( status );
    
    switch ( iCurrentState )
        {
        case EWaitingTimer:
            {
            // Timer completed, send again
            User::LeaveIfError( Send() );
            break;
            }
        case ESending:
            {
            // Send completed, start timer again
            iSequenceNum++;
            User::LeaveIfError( StartSending() );
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepalive::DoCancel
// ---------------------------------------------------------------------------
//        
void CMccRtpKeepalive::DoCancel()
    {
    if ( EWaitingTimer == iCurrentState )
        {
        __SUBCONTROLLER( "Keepalive: Cancel Timer !!!" )
        iTimer.Cancel();
        }
    else if ( ESending == iCurrentState )
        {
        __SUBCONTROLLER( "Keepalive: Cancel Sending !!!" )
        iRtpAPI.CancelSend( iRtpSessionId );
        }
    
    iCurrentState = EIdle;
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepalive::RunError
// ---------------------------------------------------------------------------
//        
TInt CMccRtpKeepalive::RunError( TInt aError )
    {
    __SUBCONTROLLER_INT1( "CMccRtpKeepalive::RunError(), err:", aError )
    __SUBCONTROLLER_INT1( "CMccRtpKeepalive::RunError(), at state:", iCurrentState )
    
    SendErrorToClient( aError );
    
    if ( aError != KErrNoMemory )
        {
        return KErrNone;
        }
    return aError;
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepalive::ResolveActivity
// ---------------------------------------------------------------------------
//
TBool CMccRtpKeepalive::ResolveActivity()
    {
    __SUBCONTROLLER( "CMccRtpKeepalive::ResolveActivity()" )
    
    // Keepalive is always active if there's only rtpsource.
    iRtpSinkExists = EFalse;
    TBool rtpSourceExists( EFalse );
    for ( TInt i = 0; i < iUsers.Count(); i++ )
        {
        if ( iUsers[ i ]->IsSink() )
            {
            iRtpSinkExists = ETrue;
            iRtpSinkIndex = i;
            }
        else
            {
            rtpSourceExists = ETrue;
            }
        }
    
    TBool keepAliveActive = ( iRtpSinkExists ) || 
                            ( !iRtpSinkExists && rtpSourceExists );
    
    __SUBCONTROLLER_INT1( "CMccRtpKeepalive::ResolveActivity(), active:", 
        keepAliveActive )
       
    return keepAliveActive;
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepalive::ToggleKeepaliveSending
// ---------------------------------------------------------------------------
//   
TInt CMccRtpKeepalive::ToggleKeepaliveSending( TBool aIsActive )
    {
    __SUBCONTROLLER_INT1( "CMccRtpKeepalive::ToggleKeepaliveSending(), active", 
        aIsActive )
    
    TInt err( KErrNone );
    if ( aIsActive )
        {
        if ( iRtpStreamId == KNullId )
            {
            __SUBCONTROLLER( "CMccRtpKeepalive::ToggleKeepaliveSending(), create rtp transmit stream" )
            
            TTranStreamParams transParams;
            transParams.iPayloadType = iRtpHeaderInfo.iPayloadType;

            if ( !iRtpSinkExists )
                {
                TRAP( err, iRtpStreamId = 
                    iRtpAPI.CreateTransmitStreamL( iRtpSessionId, 
                                                   transParams, 
                                                   iSSRC ) ); 
                }
            else
                {
                // get rtp sink id
                iRtpStreamId = iUsers[iRtpSinkIndex]->RtpStreamId();
                }    
                               
            if ( iRtpStreamId == KNullId )
                {
                err = KErrCouldNotConnect;
                }
            }
        
        if ( !err && iCurrentState == EIdle )
            {
            err = StartSending();
            }
        }
    else
        {
        Cancel();
        }
        
    __SUBCONTROLLER_INT1( "CMccRtpKeepalive::ToggleKeepaliveSending(), exit with err:", 
        err )
    
    return err;
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepalive::StartSending
// ---------------------------------------------------------------------------
//   
TInt CMccRtpKeepalive::StartSending()
    {
    __SUBCONTROLLER( "CMccRtpKeepalive::StartSending()" )
    
    TInt err( KErrNone );
        
    if ( !IsActive() && ( 0 < iKeepaliveInterval ) )
        {
        if ( iRemoteAddressSet && iRtpMediaClock )
            {
            __SUBCONTROLLER( "CMccRtpKeepalive::StartSending(), issue send timer" )
            
            if ( iPreAudioPacketSent || !iRtpSinkExists  )
                {
                iTimer.After( iStatus, iKeepaliveInterval );
                SetActive();
                iCurrentState = EWaitingTimer;
                }
            else
                {
                // Send firewall punching packet before possibly later 
                // starting audio.
                Send();
                iPreAudioPacketSent = ETrue;
                }
            }
        else
            {
            iCurrentState = ESendingPending;
            }
        }
    
    return err;
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepalive::Send
// ---------------------------------------------------------------------------
//    
TInt CMccRtpKeepalive::Send()
    {
    TInt err( KErrInUse );
    if ( !IsActive() )
        {
        // not called if rtpsink exists
        if ( !iRtpSinkExists )
            {
            __SUBCONTROLLER( "CMccRtpKeepalive::StartSending() - No RtpSink Exists" )
            iCurrentTime.UniversalTime();
            TTimeIntervalMicroSeconds interval = 
                iCurrentTime.MicroSecondsFrom( iInitialTime );
            iRtpHeaderInfo.iTimestamp = interval.Int64();
            
            err = iRtpAPI.SendRtpPacket( iRtpStreamId, 
                                         iSequenceNum,
                                         iRtpHeaderInfo, 
                                         *iKeepaliveData,
                                         iStatus );                           
            }
        else
            {
            __SUBCONTROLLER_INT1( "CMccRtpKeepalive::Send() PT:", iRtpHeaderInfo.iPayloadType )        
            __SUBCONTROLLER_INT1( "CMccRtpKeepalive::Send() Data:", iKeepaliveData->Length() )
            iRtpMediaClock->GetLatestConsumedTS( iRtpHeaderInfo.iTimestamp );        
            err = iRtpAPI.SendRtpPacket( iRtpStreamId,
                                         iRtpHeaderInfo,
                                         *iKeepaliveData,
                                         iStatus );
            }              
        if ( !err )
            {
            __SUBCONTROLLER( "CMccRtpKeepalive::StartSending - Sending" )              
            SetActive();    
            iCurrentState = ESending;
            }
        }
    return err;
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepalive::Random
// ---------------------------------------------------------------------------
//
TUint16 CMccRtpKeepalive::Random() const
    {
    const TInt KRandMax = 0x7fff;
    TTime tmp_time;
    tmp_time.HomeTime();
    TInt64 seed = tmp_time.Int64();
    TInt32 rand = Math::Rand( seed );
    return static_cast<TUint16>( Abs( rand ) % KRandMax );
    }

// ---------------------------------------------------------------------------
// CMccRtpKeepalive::SendErrorToClient
// ---------------------------------------------------------------------------
//    
void CMccRtpKeepalive::SendErrorToClient( TInt aError )
    {
    __SUBCONTROLLER( "CMccRtpKeepalive::SendErrorToClient()" )
    
    TMccEvent event;
    event.iErrorCode = aError;
    event.iEventCategory = KMccEventCategoryRtp;
    event.iEventType = KMccStreamError;
    
    // Use whatever internal uid
    TMccInternalEvent internalEvent( KMccRtpSourceUid, 
                             EMccInternalEventNone,
                             event );
                             
    iEventHandler.SendEventToClient( internalEvent );
    
    __SUBCONTROLLER( "CMccRtpKeepalive::SendErrorToClient(), exit" )
    }
        
// ---------------------------------------------------------------------------
// CMccRtpKeepalive::CMccRtpKeepalive
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CMccRtpKeepalive::CMccRtpKeepalive( 
    MAsyncEventHandler& aEventHandler, 
    CRtpAPI& aRtpAPI,
    TRtpId aRtpSessionId,
    TBool aRemoteAddressSet ) : 
    CActive( EPriorityStandard ),
    iEventHandler( aEventHandler ), 
    iRtpAPI( aRtpAPI ),
    iRtpSessionId( aRtpSessionId ),
    iRtpStreamId( KNullId ),
    iRemoteAddressSet( aRemoteAddressSet )
    {  
    CActiveScheduler::Add( this );
    }
      
// ---------------------------------------------------------------------------
// CMccRtpKeepalive::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CMccRtpKeepalive::ConstructL( 
    TUint8 aKeepalivePayloadType,
    TUint32 aKeepaliveInterval,
    const TDesC8& aKeepaliveData )
    {
    __SUBCONTROLLER( "CMccRtpKeepalive::ConstructL()" )
    
    User::LeaveIfError( iTimer.CreateLocal() );
    
    __ASSERT_ALWAYS( aKeepalivePayloadType < KMccPayloadTypeMax, 
                     User::Leave( KErrArgument ) );
    
    iRtpHeaderInfo.iMarker = 0;
    
    UpdateParamsL( aKeepalivePayloadType, aKeepaliveInterval, 
        aKeepaliveData, NULL );
    
    iSequenceNum = Random();
    
    iInitialTime.UniversalTime();
    
    __SUBCONTROLLER_INT1( "CMccRtpKeepalive::ConstructL(), keepalive pt:", 
                          aKeepalivePayloadType )
    __SUBCONTROLLER_INT1( "CMccRtpKeepalive::ConstructL(), keepalive interval (microsecs):", 
                          iKeepaliveInterval )
    __SUBCONTROLLER_INT1( "CMccRtpKeepalive::ConstructL(), keepalive data len:", 
                          iKeepaliveData->Length() )
    __SUBCONTROLLER( "CMccRtpKeepalive::ConstructL(), exit" )
    }
    


       
// ========================== OTHER EXPORTED FUNCTIONS =======================

//  End of File  
