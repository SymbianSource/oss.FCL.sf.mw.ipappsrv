/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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



// INCLUDE FILES
#include "mccvideosourcekeyretriever.h"
#include "mccinternalcodecs.h"
#include "mccinternaldef.h"
#include "mccinternalevents.h"
#include "mccvideosourceimpl.h"
#include "mccvideosourcesinklogs.h"
#include "mccperiodicrunner.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccVideoSourceConfigKeyRetriever::NewLC
// -----------------------------------------------------------------------------
//
CMccVideoSourceConfigKeyRetriever* CMccVideoSourceConfigKeyRetriever::NewLC( 
    MMccCameraHandler& aCameraHandler )
    {
    CMccVideoSourceConfigKeyRetriever* self = 
        new ( ELeave ) CMccVideoSourceConfigKeyRetriever( aCameraHandler );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceConfigKeyRetriever::~CMccVideoSourceConfigKeyRetriever
// -----------------------------------------------------------------------------
//    
CMccVideoSourceConfigKeyRetriever::~CMccVideoSourceConfigKeyRetriever()
    {
    __V_SOURCESINK_CONTROLL( 
    "CMccVideoSourceConfigKeyRetriever::~CMccVideoSourceConfigKeyRetriever" )
    
    delete iGuardTimer;
    
    StopWaiting();

    delete iWait;
    
    delete iSource;
    
    delete iBuffer;
    
    __V_SOURCESINK_CONTROLL( 
    "CMccVideoSourceConfigKeyRetriever::~CMccVideoSourceConfigKeyRetriever, exit" )
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceConfigKeyRetriever::RetrieveL
// -----------------------------------------------------------------------------
//    
void CMccVideoSourceConfigKeyRetriever::RetrieveL( 
    const TMccCodecInfo& aVideoCodec, 
    TDes8& aConfigKey )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceConfigKeyRetriever::RetrieveL" )
    
    __ASSERT_ALWAYS( aVideoCodec.iSdpName.CompareF( KAVCSdpName ) == 0, 
                     User::Leave( KErrNotSupported ) );
    
    delete iBuffer;
    iBuffer = NULL;
    iBuffer = CMMFDataBuffer::NewL( aVideoCodec.iFrameSize );
        
    delete iSource;
    iSource = NULL;
    iSource = CMccVideoSourceImpl::NewL( 0, NULL, ETrue );
    
    iFailed = EFalse;

    iSource->SourceThreadLogon( *this );
    iSource->SetCameraHandler( iCameraHandler );
    iSource->SetVideoCodecL( aVideoCodec );
    iSource->SourcePrimeL( EFalse );

    if ( !iSource->HasConfigKey() && !iFailed )
        {
        __V_SOURCESINK_CONTROLL( "Start waiting for key" )
        
        // Start also guard timer to prevent possible eternal jam
        StartGuardTimer();
        
        iWait->Start();
    
        __V_SOURCESINK_CONTROLL( "Waiting completed" )
        StopGuardTimer();
        }
    
    __ASSERT_ALWAYS( !iFailed, User::Leave( KErrNotFound ) );
    
    iSource->GetConfigKeyL( aVideoCodec, aConfigKey, EFalse );
    
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceConfigKeyRetriever::RetrieveL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceConfigKeyRetriever::SendEventToClient
// -----------------------------------------------------------------------------
//           
TInt CMccVideoSourceConfigKeyRetriever::SendEventToClient( const TMMFEvent& aEvent )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceConfigKeyRetriever::SendEventToClient" )
    
    TInt error( KErrNone );
   	TMccEvent* mccEventPtr = NULL;
	if ( IS_MCC_EVENT( aEvent ) )
	    {
	    mccEventPtr = 
	        reinterpret_cast<const TMccInternalEvent&>( aEvent ).iMccEvent;                                     
	    }
	
	if ( mccEventPtr )
	    {
	    TRAP( error, HandleEventL( *mccEventPtr ) );
	    if ( error != KErrNone )
	        {
	        HandleError();
	        }
	    }
	    
	__V_SOURCESINK_CONTROLL( "CMccVideoSourceConfigKeyRetriever::SendEventToClient, exit" )    
	
	return error;
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceConfigKeyRetriever::SinkDataTypeCode
// -----------------------------------------------------------------------------
//    
TFourCC CMccVideoSourceConfigKeyRetriever::SinkDataTypeCode( TMediaId /*aMediaId*/ )
    {
    return TFourCC( 0 );
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceConfigKeyRetriever::EmptyBufferL
// -----------------------------------------------------------------------------
//    
void CMccVideoSourceConfigKeyRetriever::EmptyBufferL( 
    CMMFBuffer* /*aBuffer*/, MDataSource* /*aSupplier*/, TMediaId /*aMediaId*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceConfigKeyRetriever::BufferFilledL
// -----------------------------------------------------------------------------
//    
void CMccVideoSourceConfigKeyRetriever::BufferFilledL( CMMFBuffer* /*aBuffer*/ )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceConfigKeyRetriever::BufferFilledL" )
    
    // First buffer was received from mediarecorder, config info should be now
    // available.
    StopWaiting();
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceConfigKeyRetriever::CanCreateSinkBuffer
// -----------------------------------------------------------------------------
//    
TBool CMccVideoSourceConfigKeyRetriever::CanCreateSinkBuffer()
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceConfigKeyRetriever::CreateSinkBufferL
// -----------------------------------------------------------------------------
//    
CMMFBuffer* CMccVideoSourceConfigKeyRetriever::CreateSinkBufferL(
    TMediaId /*aMediaId*/, TBool& /*aReference*/ )
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceConfigKeyRetriever::ConstructSinkL
// -----------------------------------------------------------------------------
//    
void CMccVideoSourceConfigKeyRetriever::ConstructSinkL( const TDesC8& /*aInitData*/ )
    {
    }


// -----------------------------------------------------------------------------
// CMccVideoSourceConfigKeyRetriever::ConstructL
// -----------------------------------------------------------------------------
//
void CMccVideoSourceConfigKeyRetriever::ConstructL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceConfigKeyRetriever::ConstructL" )
    
    iWait = new ( ELeave ) CActiveSchedulerWait;
    
    iGuardTimer = CMccPeriodicRunner::NewL();
    
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceConfigKeyRetriever::ConstructL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceConfigKeyRetriever::ConstructL
// -----------------------------------------------------------------------------
//    
CMccVideoSourceConfigKeyRetriever::CMccVideoSourceConfigKeyRetriever( 
    MMccCameraHandler& aCameraHandler ) :
    MDataSink( KNullUid ),
    iCameraHandler( aCameraHandler ),
    iGuardTimerTimeout( KMccKeyRetrieveGuardTimeoutMicrosecs )
    {
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceConfigKeyRetriever::HandleEventL
// -----------------------------------------------------------------------------
//
void CMccVideoSourceConfigKeyRetriever::HandleEventL( TMccEvent& aEvent )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceConfigKeyRetriever::HandleEventL" )
    
    if ( aEvent.iEventType == KMccStreamError || aEvent.iErrorCode != KErrNone )
        {
        User::Leave( KErrNotReady );
        }
    else if ( aEvent.iEventType == KMccStreamPrepared )
        {
        __V_SOURCESINK_CONTROLL( "Prepared" )
        
        // Start recording, first buffer received from mediarecorder
	    // should contain config info         
        iSource->FillBufferL( iBuffer, this, KUidMediaTypeVideo );
	    iSource->SourcePlayL();
        }
    else if ( aEvent.iEventType == KMccStreamStarted )
        {
        __V_SOURCESINK_CONTROLL( "Started" )
        }
    else
        {
        // NOP
        }
        
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceConfigKeyRetriever::HandleEventL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceConfigKeyRetriever::HandleError
// -----------------------------------------------------------------------------
//    
void CMccVideoSourceConfigKeyRetriever::HandleError()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceConfigKeyRetriever::HandleError" )
    
    iFailed = ETrue;
    StopWaiting();
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceConfigKeyRetriever::StopWaiting
// -----------------------------------------------------------------------------
//
void CMccVideoSourceConfigKeyRetriever::StopWaiting()
    {
    if ( iWait && iWait->IsStarted() )
        {
        __V_SOURCESINK_CONTROLL( "CMccVideoSourceConfigKeyRetriever::StopWaiting" )
        iWait->AsyncStop();
        }
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceConfigKeyRetriever::StartGuardTimer
// -----------------------------------------------------------------------------
//
void CMccVideoSourceConfigKeyRetriever::StartGuardTimer()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceConfigKeyRetriever::StartGuardTimer" )
    
    iGuardTimer->Start( 
        iGuardTimerTimeout, TCallBack( GuardTimerExpired, this ) );	
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceConfigKeyRetriever::StopGuardTimer
// -----------------------------------------------------------------------------
//
void CMccVideoSourceConfigKeyRetriever::StopGuardTimer()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceConfigKeyRetriever::StopGuardTimer" )
    
    iGuardTimer->Stop();	
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceConfigKeyRetriever::GuardTimerExpired
// -----------------------------------------------------------------------------
//
TInt CMccVideoSourceConfigKeyRetriever::GuardTimerExpired( TAny* aObject )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceConfigKeyRetriever::GuardTimerExpired" )
    
    CMccVideoSourceConfigKeyRetriever* self = 
        static_cast<CMccVideoSourceConfigKeyRetriever*>( aObject );
    if ( self )
        {
        self->StopWaiting();
        }
    
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceConfigKeyRetriever::GuardTimerExpired, exit" )
    
    return KErrNone;
    }
    
// End of file

