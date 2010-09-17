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
#include <mmcccodecinformationfactory.h>
#include <mmcccodecinformation.h>
#include "mccfilesink.h"
#include "mmccinterfacedef.h"
#include "mmcccodecinformation.h"
#include "mccfilesinklogs.h"
#include "mccinternalevents.h"
#include "mccinternaldef.h"
#include "mccresources.h"

// CONSTANTS

const TInt KMccMaxNumTimestamps = 5;

// used for Calculate Average Timestamp
const TInt KMccDefaultAvgTimestampDiff = 66000;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccFileSink::NewSinkL
// -----------------------------------------------------------------------------
//
MDataSink* CMccFileSink::NewSinkL( TUid /*aImplementationUid*/, 
                                   const TDesC8& /*aInitData*/ )
    {
    __FILESINK_CONTROLL( "CMccFileSink::NewSinkL" )
    CMccFileSink* self = new ( ELeave ) CMccFileSink();
    return static_cast<MDataSink*>( self );
    }

// -----------------------------------------------------------------------------
// CMccFileSink::ConstructSinkL
// -----------------------------------------------------------------------------
//
void CMccFileSink::ConstructSinkL( const TDesC8& aInitData )
    {
    __FILESINK_CONTROLL( "CMccFileSink::ConstructSinkL" )

	iFileComposer = CCamC3GPDataSink::NewL( this );
    iFileComposer->SetSizeLimit( iMaxFileSize );
    
    TPckgBuf<TFileName> initData;
    initData.Copy( aInitData );
    iFileName = initData();
    
    }
    
// -----------------------------------------------------------------------------
// CMccFileSink::CMccFileSink
// -----------------------------------------------------------------------------
//
CMccFileSink::CMccFileSink() : 
    CMccDataSink( KMccFileSinkUid ),
    iCurrAvgTimestampDifference( KMccDefaultAvgTimestampDiff )
    {
    iVideoCodec.iFourCC = TFourCC( KMccFourCCIdH263 );
    // FJKI-7J58CB no use case for audio in file, hence removing the audio track capability 
    //             per request from customer
    // iAudioFourCC = TFourCC( KMccFourCCIdAMRNB );
    iAudioFourCC = TFourCC( KMMFFourCCCodeNULL );
    iFileName = KNullDesC;
    iMaxFileSize = 0; 
    iT1 = 0;
    iT2 = 0;
    iPausedDuration = 0;
    iPreviousTimestamp = 0;
    iAddToTimestamp = 0;
    iSizeLimitReached = EFalse;
    iNotifySizeLimitReached = EFalse;
    SetActiveUserIndex( 0 );
    }
        
// -----------------------------------------------------------------------------
// CMccFileSink::~CMccFileSink
// -----------------------------------------------------------------------------
//
CMccFileSink::~CMccFileSink()
    {
    __FILESINK_CONTROLL( "CMccFileSink::~CMccFileSink" )
    // Cleanup and other ending operations...
    if ( iCurrentState == ERecording || iCurrentState == EPaused )
        {
        TRAP_IGNORE( CMccFileSink::SinkStopL() );
        }	
	delete iFileComposer;
	iTimestamps.Close();
	
	iUsers.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMccFileSink::SetCurrentUser
// -----------------------------------------------------------------------------
//
void CMccFileSink::SetCurrentUser( MAsyncEventHandler* aEventHandler )
    {
    iAsyncEventHandler = aEventHandler;
    }

// -----------------------------------------------------------------------------
// CMccFileSink::SinkPrimeL
// -----------------------------------------------------------------------------
//
void CMccFileSink::SinkPrimeL()
	{
	__FILESINK_CONTROLL( "CMccFileSink::SinkPrimeL" )

    DoSinkPrimeL();	
	}
	
// -----------------------------------------------------------------------------
// CMccFileSink::SinkPlayL()
// -----------------------------------------------------------------------------
//
void CMccFileSink::SinkPlayL()
    {
    __FILESINK_CONTROLL( "CMccFileSink::SinkPlayL" )  
    
    DoSinkPlayL();
    }

// -----------------------------------------------------------------------------
// CMccFileSink::SinkPauseL()
// 
// Pauses streaming by cancelling timers
// -----------------------------------------------------------------------------
//
void CMccFileSink::SinkPauseL()
    {
    __FILESINK_CONTROLL( "CMccFileSink::SinkPauseL" )

    DoSinkPauseL();
	}

// -----------------------------------------------------------------------------
// CMccFileSink::SinkStopL()
// 
// Stops streaming
// -----------------------------------------------------------------------------
//
void CMccFileSink::SinkStopL()
    {
    __FILESINK_CONTROLL( "CMccFileSink::SinkStopL" )
    
    DoSinkStopL();		
    }

// -----------------------------------------------------------------------------
// CMccFileSink::SinkDataTypeCode
// -----------------------------------------------------------------------------
//
TFourCC CMccFileSink::SinkDataTypeCode( TMediaId aMediaId )
	{
    __FILESINK_CONTROLL( "CMccFileSink::SinkDataTypeCode" )

    if ( KUidMediaTypeVideo == aMediaId.iMediaType )
        {
        return iVideoCodec.iFourCC;
        }
    else if ( KUidMediaTypeAudio == aMediaId.iMediaType  )
        {
        return iAudioFourCC;
        }
    else
        {
        return TFourCC( KMMFFourCCCodeNULL );
        }
	}
	
// -----------------------------------------------------------------------------
// CMccFileSink::SetSinkDataTypeCode
// -----------------------------------------------------------------------------
//
TInt CMccFileSink::SetSinkDataTypeCode( TFourCC aCodec, 
                            TMediaId aMediaId )
	{
    __FILESINK_CONTROLL( "CMccFileSink::SetSinkDataTypeCode" )

    TInt retVal = KErrNone;
    if ( KUidMediaTypeVideo == aMediaId.iMediaType &&
    	aCodec == iVideoCodec.iFourCC )
        {
        retVal = KErrNone;
        }
    else if ( KUidMediaTypeAudio == aMediaId.iMediaType &&
    	aCodec == iAudioFourCC )
        {
        retVal = KErrNone;
        }
    else
        {
        retVal = KErrNotSupported;
        }

	return retVal;
	}	

// -----------------------------------------------------------------------------
// CMccFileSink::BufferEmptiedL
// -----------------------------------------------------------------------------
//
void CMccFileSink::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
	{
    __FILESINK_MEDIA( "CMccFileSink::BufferEmptiedL" )
    User::Leave( KErrNotSupported );
	}
	
// -----------------------------------------------------------------------------
// CMccFileSink::CanCreateSinkBuffer
// -----------------------------------------------------------------------------
//
TBool CMccFileSink::CanCreateSinkBuffer()
	{
    __FILESINK_CONTROLL( "CMccFileSink::CanCreateSinkBuffer, EFalse" )
	return EFalse;
	}	

// -----------------------------------------------------------------------------
// CMccFileSink::CreateSinkBufferL
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccFileSink::CreateSinkBufferL( 
	TMediaId /*aMediaId*/, 
    TBool& /*aReference*/ )
	{
	__FILESINK_CONTROLL( "CMccFileSink::CreateSinkBufferL, return NULL" )
    CMMFBuffer* buffer = NULL;
	User::Leave( KErrNotSupported );	
	return buffer;	
	}
	
// -----------------------------------------------------------------------------
// CMccFileSink::SinkThreadLogon
// -----------------------------------------------------------------------------
//
TInt CMccFileSink::SinkThreadLogon( MAsyncEventHandler& aEventHandler )
	{
    __FILESINK_CONTROLL( "CMccFileSink::SinkThreadLogon" )

    TInt err( KErrNone );
    TMccFileSinkUser* userEntry = 
        MccUserArray<TMccFileSinkUser>::FindUserEntryForCurrent( iUsers, &aEventHandler );
    if ( !userEntry )
        {
        TRAP( err, AddUserL( &aEventHandler ) );
        }
    
    __FILESINK_CONTROLL_INT1( 
        "CMccFileSink::SinkThreadLogon, exit with err:", err )
        
    return err;
	}
	
// -----------------------------------------------------------------------------
// CMccFileSink::SinkThreadLogoff
// -----------------------------------------------------------------------------
//
void CMccFileSink::SinkThreadLogoff()
	{
	__FILESINK_CONTROLL( "CMccFileSink::SinkThreadLogoff" )

    // If multiple different codecs are using the filesink and active user
    // is removed, reset active user information so that new active user
    // can be seleceted once someone else tries to use the sink
    if ( IsActiveUser( iAsyncEventHandler ) )
        {
        SetActiveUserIndex( KErrNotFound );
        }
         
    MccUserArray<TMccFileSinkUser>::RemoveCurrentUser( iUsers, iAsyncEventHandler );
    
    if ( iUsers.Count() > 0 )
        {
        SetCurrentUser( iUsers[ 0 ]->iEventHandler );
        }
    else
        {
        SetCurrentUser( NULL );
        }
	}

// -----------------------------------------------------------------------------
// CMccFileSink::EmptyBufferL
// -----------------------------------------------------------------------------
//
void CMccFileSink::EmptyBufferL( CMMFBuffer* aBuffer,
                  MDataSource* aProvider,
                  TMediaId aMediaId )
	{
	__FILESINK_MEDIA( "CMccFileSink::EmptyBufferL" )
	
	__ASSERT_ALWAYS( aBuffer && aProvider, User::Leave( KErrArgument ) );
	
	
    if ( iCurrentState != ERecording )
        {
        __FILESINK_MEDIA( "CMccFileSink::EmptyBufferL, IGNORED" )
        
        // Do not let the datapath to halt
        aProvider->BufferEmptiedL( aBuffer );
        return;
        }

    if ( iNotifySizeLimitReached )
        {
        __FILESINK_MEDIA( 
            "CMccFileSink::EmptyBufferL, IGNORED (notify size limit reached)" )
        MfcoSizeLimitReachedL();
        
        // Do not let the datapath to halt
        aProvider->BufferEmptiedL( aBuffer );
        return;
        }
        
	CMMFDataBuffer* buffer = static_cast<CMMFDataBuffer*>(aBuffer);
	CCMRMediaBuffer* outBuffer = NULL; 
	                  
	TTimeIntervalMicroSeconds timeToPlay = TimeToPlayL( buffer->TimeToPlay() );
    	    	
   	__FILESINK_MEDIA_INT1( "CMccFileSink::EmptyBufferL, timeToPlay = ", 
	                       timeToPlay.Int64() )
	
	TBool dropBuffer( EFalse );                      
	if ( aMediaId.iMediaType == KUidMediaTypeAudio )
		{
    	__FILESINK_MEDIA( "CMccFileSink::EmptyBufferL, audio" )
    	    
    	outBuffer = new ( ELeave ) CCMRMediaBuffer(
        	buffer->Data(), CCMRMediaBuffer::EAudioAMRNB, buffer->BufferSize(),
            ETrue,  timeToPlay ); 
        CleanupStack::PushL( outBuffer );  
		}
	else if ( aMediaId.iMediaType == KUidMediaTypeVideo )
		{
    	__FILESINK_MEDIA( "CMccFileSink::EmptyBufferL, video" )    
        
        TFourCC providerDataType = UpdateActiveUserL( aMediaId, *aProvider );
        
        CCMRMediaBuffer::TBufferType bufType = 
            ResolveBufferType( *buffer, providerDataType );	
        
        dropBuffer = CheckWritingPermission( *buffer, bufType );
        
    	outBuffer = new ( ELeave ) CCMRMediaBuffer(
        	buffer->Data(), bufType, buffer->BufferSize(),
            ETrue,  timeToPlay );    
            
        CleanupStack::PushL( outBuffer );   
		}
	else
		{
    	__FILESINK_MEDIA( "CMccFileSink::EmptyBufferL, unknown media" ) 
		User::Leave( KErrNotSupported );	
		}
    
    if ( !dropBuffer )
        {
    	iFileComposer->WriteBufferL( outBuffer );
        }
        
	CleanupStack::PopAndDestroy( outBuffer );
	outBuffer= NULL;
	aProvider->BufferEmptiedL( aBuffer );
	}	

// -----------------------------------------------------------------------------
// CMccFileSink::BufferFilledL
// -----------------------------------------------------------------------------
//
void CMccFileSink::BufferFilledL( CMMFBuffer* /*aBuffer*/ )
	{
	__FILESINK_MEDIA( "CMccFileSink::BufferFilledL" )
	}	                  

// -----------------------------------------------------------------------------
// CMccFileSink::MfcoDiskFullL
// -----------------------------------------------------------------------------
//
void CMccFileSink::MfcoDiskFullL()
	{
	__FILESINK_MEDIA( "CMccFileSink::MfcoDiskFullL" )
	
	// After size limit has been reached, there is no way to really continue
	// recording to the same file, writebuffer returns without error but
	// has no effect. Pause the sink anyway but fail if resume is tried.
	// Creating new filewriter would be only way to recover from this but
	// that would overwrite old recording.
	iSizeLimitReached = ETrue;
	
	SendStreamEventToClient( KMccResourceNotAvailable, KErrNoMemory, ETrue );
	
    TRAP_IGNORE( AutomaticPauseL() );
    
    iNotifySizeLimitReached = EFalse;
	}	                  

// -----------------------------------------------------------------------------
// CMccFileSink::MfcoSizeLimitReachedL
// -----------------------------------------------------------------------------
//
void CMccFileSink::MfcoSizeLimitReachedL()
	{
	__FILESINK_MEDIA( "CMccFileSink::MfcoSizeLimitReachedL" )
	
	iSizeLimitReached = ETrue;
	
	SendStreamEventToClient( KMccResourceNotAvailable, KErrNone, ETrue );

    TRAP_IGNORE( AutomaticPauseL() );
    
    iNotifySizeLimitReached = EFalse;
	}	                  

// -----------------------------------------------------------------------------
// CMccFileSink::SetVideoCodecL
// -----------------------------------------------------------------------------
//
void CMccFileSink::SetVideoCodecL( const TMccCodecInfo& aVideoCodec )
    {
   	__FILESINK_CONTROLL_STR8( 
	    "CMccFileSink::SetVideoCodecL, sdpname:", aVideoCodec.iSdpName )
    
    TMccFileSinkUser* user = 
        MccUserArray<TMccFileSinkUser>::FindUserEntryForCurrent( iUsers, iAsyncEventHandler );
    if ( user )
        {
        user->iCodecInfo = aVideoCodec;
        }
    }

// -----------------------------------------------------------------------------
// CMccFileSink::SetAudioCodecL
// -----------------------------------------------------------------------------
//
void CMccFileSink::SetAudioCodecL( const TMccCodecInfo& /*aAudioCodec*/ )
    {
    __FILESINK_MEDIA( "CMccFileSink::SetAudioCodecL" )
    }
    
// -----------------------------------------------------------------------------
// CMccFileSink::RecordTimeAvailableL
// -----------------------------------------------------------------------------
//
void CMccFileSink::RecordTimeAvailableL( TTimeIntervalMicroSeconds& aTime )
	{ 
	__FILESINK_CONTROLL( "CMccFileSink::RecordTimeAvailableL" )
	aTime = iFileComposer->GetRemainingTimeL();
	}

// -----------------------------------------------------------------------------
// CMccFileSink::SetFileNameL
// -----------------------------------------------------------------------------
//
void CMccFileSink::SetFileNameL( const TFileName aFileName )
	{
	__FILESINK_CONTROLL( "CMccFileSink::SetFileNameL" )
	iFileName = aFileName;
	}
	
// -----------------------------------------------------------------------------
// CMccFileSink::SendStreamEventToClient
// -----------------------------------------------------------------------------
//	
void CMccFileSink::SendStreamEventToClient( 
    TMccEventType aEventType, 
    TInt aError,
    TBool aToAllClients )
	{
	__FILESINK_CONTROLL_INT2( 
	    "CMccFileSink::SendStreamEventToClient, type", aEventType, 
	    " to all:", aToAllClients )
	
    TMccEvent event( 0, 
                     0, 
                     0, 
                     MCC_ENDPOINT_ID( static_cast<MDataSink*>( this ) ), 
                     KMccEventCategoryStream, 
                     aEventType, 
                     aError, 
                     KNullDesC8 );

    if ( aToAllClients )
        {
        for ( TInt i = 0; i < iUsers.Count(); i++ )
            {
            FinalizeSendEvent( iUsers[ i ]->iEventHandler, event );
            }
        }
    else
        {
        FinalizeSendEvent( iAsyncEventHandler, event );
        }
	}
	
// -----------------------------------------------------------------------------
// CMccFileSink::TimeToPlayL
// -----------------------------------------------------------------------------
//	   	
TTimeIntervalMicroSeconds CMccFileSink::TimeToPlayL( 
    TTimeIntervalMicroSeconds aCurrentTimestamp )
    { 
    CalculateAverageTimestampDifferenceL( aCurrentTimestamp );

    TTimeIntervalMicroSeconds timeToPlay( 0 );
    
    if ( aCurrentTimestamp >= iPausedDuration )
        {
        __FILESINK_CONTROLL("CMccFileSink::TimeToPlay aCurrentTimestamp \
>= iPausedDuration" )

        timeToPlay = aCurrentTimestamp.Int64() - iPausedDuration.Int64();    
        }
    else
        {
        __FILESINK_CONTROLL("CMccFileSink::TimeToPlay aCurrentTimestamp \
< iPausedDuration" )
        timeToPlay = aCurrentTimestamp; 
        }
   
    timeToPlay = ( timeToPlay.Int64() + iAddToTimestamp );
    
    __FILESINK_CONTROLL_INT1("CMccFileSink::TimeToPlay, \
timeToPlay=", timeToPlay.Int64() )  

    iPreviousTimestamp = aCurrentTimestamp;
  
    return timeToPlay;
    }

// -----------------------------------------------------------------------------
// CMccFileSink::CalculateAverageTimestampDifferenceL
// -----------------------------------------------------------------------------
//  
void CMccFileSink::CalculateAverageTimestampDifferenceL( 
    const TTimeIntervalMicroSeconds& aCurrentTimestamp )
    {
    TInt64 averageTimeStampDifference = 0;
    if ( iTimestamps.Count() == KMccMaxNumTimestamps )
       {
       
       for ( TInt i = iTimestamps.Count() - 1; i > 0; i-- )
               {
               averageTimeStampDifference += ( iTimestamps[ i ] - iTimestamps[ i - 1 ] );
               }
               
       averageTimeStampDifference = averageTimeStampDifference / ( KMccMaxNumTimestamps - 1 );
       }
    if ( averageTimeStampDifference > 0 )
	   {
	   iCurrAvgTimestampDifference = averageTimeStampDifference;
       }
    
    if ( aCurrentTimestamp > iPreviousTimestamp )
       {
       if ( iTimestamps.Count() >= KMccMaxNumTimestamps )
           {
           iTimestamps.Remove( 0 );
           }
       iTimestamps.AppendL( aCurrentTimestamp.Int64() );
       }
    else if ( aCurrentTimestamp < iPreviousTimestamp )
       {
	   TInt64 currDifference = iPreviousTimestamp.Int64() - aCurrentTimestamp.Int64();
       iAddToTimestamp += ( currDifference + iCurrAvgTimestampDifference );
       iTimestamps.Reset();
       iPausedDuration = 0;
           
       __FILESINK_CONTROLL_INT1("CMccFileSink::TimeToPlay, iAddToTimestamp=", iAddToTimestamp )  
       }
    else
        {
        // NOP
        }
    }

// -----------------------------------------------------------------------------
// CMccFileSink::ResetTimers
// -----------------------------------------------------------------------------
//	  
void CMccFileSink::ResetTimers()
    {
    iT1 = 0;
    iT2 = 0; 
    iPausedDuration = 0;  
    iPreviousTimestamp = 0;
    iAddToTimestamp = 0;
    iTimestamps.Reset();
    }

// -----------------------------------------------------------------------------
// CMccFileSink::SetPausedDuration
// -----------------------------------------------------------------------------
//	
void CMccFileSink::SetPausedDuration( TTime aT1, TTime aT2 )
    {
    __FILESINK_CONTROLL( "CMccFileSink::SetPausedDuration" )
    __FILESINK_CONTROLL_INT2( 
        "CMccFileSink::SetPausedDuration, aT1=", aT1.Int64(),
        "aT2=", aT2.Int64() )

    if ( aT1 != 0 && aT1 < aT2 )
        {
        __FILESINK_CONTROLL_INT1("CMccFileSink::SetPausedDuration, \
before iPausedDuration=", iPausedDuration.Int64() ) 

        iPausedDuration = ( iPausedDuration.Int64() + 
                            aT2.MicroSecondsFrom( aT1 ).Int64() );
                            
        __FILESINK_CONTROLL_INT1("CMccFileSink::SetPausedDuration, \
after iPausedDuration=", iPausedDuration.Int64() ) 
        }
    }

// -----------------------------------------------------------------------------
// CMccFileSink::AutomaticPauseL
// -----------------------------------------------------------------------------
//
void CMccFileSink::AutomaticPauseL()
    {
    __FILESINK_CONTROLL( "CMccFileSink::AutomaticPauseL" )
    
    for ( TInt i = 0; i < iUsers.Count(); i++ )
        {
        TMccEvent* controlEvent = new ( ELeave ) TMccEvent;
        CleanupStack::PushL( controlEvent );
        
        controlEvent->iEndpointId = MCC_ENDPOINT_ID( static_cast<MDataSink*>( this ) );
    	controlEvent->iEventCategory = KMccEventCategoryStreamControl;
    	controlEvent->iEventType = KMccStreamPaused;
    	controlEvent->iEventNumData = KMccAutomaticEvent;

        User::LeaveIfError( 
            FinalizeSendEvent( iUsers[ i ]->iEventHandler, *controlEvent ) );
        
        CleanupStack::PopAndDestroy( controlEvent );
        }
    
    __FILESINK_CONTROLL( "CMccFileSink::AutomaticPauseL, exit" )
    }  
	
// -----------------------------------------------------------------------------
// CMccFileSink::FinalizeSendEvent
// -----------------------------------------------------------------------------
//	
TInt CMccFileSink::FinalizeSendEvent( 
    MAsyncEventHandler* aEventHandler, 
    TMccEvent& aEvent )
	{
	TInt err( KErrNone );
	if ( aEventHandler )
	    {
	    TMccInternalEvent internalEvent( KMccFileSinkUid, 
		                                 EMccInternalEventNone,
		                                 aEvent );
		                         
		err = aEventHandler->SendEventToClient( internalEvent );
	    }
	else
		{
		__FILESINK_CONTROLL( "CMccFileSink::FinalizeSend, aEventHandler=NULL" )
		err = KErrNotReady;
		}
    return err;
	}

// ---------------------------------------------------------------------------
// CMccFileSink::GetCodecTypeStringLC
// ---------------------------------------------------------------------------
//
HBufC8* CMccFileSink::GetCodecTypeStringLC( const TMccCodecInfo& aCodecInfo )    
    {
    __FILESINK_CONTROLL( "CMccFileSink::GetCodecTypeStringLC" )
    
    CMccCodecInformationFactory* factory = CMccCodecInformationFactory::NewL();
    CleanupStack::PushL( factory );
    CMccCodecInformation* codec = 
        factory->CreateCodecInformationL( aCodecInfo.iSdpName );
    CleanupStack::PushL( codec );
    codec->SetValues( aCodecInfo );
    HBufC8* fmtp = codec->GetFmtpL().AllocL();
    CleanupStack::PopAndDestroy( codec );
    CleanupStack::PopAndDestroy( factory );
    CleanupStack::PushL( fmtp );
    
    _LIT8( KMccCodecTypeFormat, "video/%S; %S" );
    HBufC8* codecType = HBufC8::NewL( 
        KMccCodecTypeFormat().Length() + fmtp->Length() + KMaxSdpNameLength );
        
    // Disabling PC-lint warnings 1025 and 64, which seems to be false warnings
    /*lint -e1025 -e64*/  
    codecType->Des().AppendFormat( KMccCodecTypeFormat(), &aCodecInfo.iSdpName, &*fmtp ); 
    CleanupStack::PopAndDestroy( fmtp );
    CleanupStack::PushL( codecType );
     
    __FILESINK_CONTROLL_STR8( "type string:", *codecType )
    
    return codecType;
    }

// ---------------------------------------------------------------------------
// CMccFileSink::ResolveBufferType
// Dec spec info cannot be given multiple times for file writer so ignore
// all later dec spec info buffers. H264 bytestream buffers needs to be
// ignored until dec spec info buffer has been written.
// ---------------------------------------------------------------------------
//
CCMRMediaBuffer::TBufferType CMccFileSink::ResolveBufferType( 
    CMMFDataBuffer& aBuffer,
    TFourCC aDataType )
    { 
    CCMRMediaBuffer::TBufferType type = CCMRMediaBuffer::EVideoH263;
    if ( aDataType == KMccFourCCIdAVC )
        {
        if ( TMccCodecInfo::IsAvcPpsOrSpsData( aBuffer.Data() ) )
            {
            __FILESINK_CONTROLL( "pps or sps" )
            type = CCMRMediaBuffer::EVideoH264BytestreamDecSpecInfo;
            }
        else
            {
            type = CCMRMediaBuffer::EVideoH264Bytestream;
            }
        }
    
    __FILESINK_CONTROLL_INT1( "CMccFileSink::ResolveBufferType, type:", type )
    
    return type;
    }

// ---------------------------------------------------------------------------
// CMccFileSink::CheckWritingPermissionL
// 1) H264 dec spec info can be written only once to the file.
// 2) Normal frames should not be written until IFrame has been written to file
// as it will cause bad quality for the clip. 
// 3) H264 bytestream cannot be written before dec spec info has been written.
// ---------------------------------------------------------------------------
//
TBool CMccFileSink::CheckWritingPermission( 
    CMMFDataBuffer& aBuffer,
    const CCMRMediaBuffer::TBufferType& aBufferType )
    { 
    TBool ignoreBuffer( EFalse );

    if ( aBufferType == CCMRMediaBuffer::EVideoH264BytestreamDecSpecInfo )
        {
        if ( !iDecSpecInfoProvided )
            {
            iDecSpecInfoProvided = ETrue;
            ignoreBuffer = EFalse;
            }
        else
            {
            __FILESINK_CONTROLL( "Ignore as dec spec info already provided!" )
            ignoreBuffer = ETrue;
            }
        }
    else if ( iMccResources && !iKeyFrameProvided )
        {
        TBool keyFrame = iMccResources->IsKeyFrame( 
                MCC_ENDPOINT_ID( static_cast<MDataSink*>( this ) ), aBuffer );
        if ( keyFrame )
            {
            __FILESINK_CONTROLL_INT1( "Key frame match for timestamp:", 
                                      aBuffer.TimeToPlay().Int64() )
            iKeyFrameProvided = ETrue;
            }
        else
            {
            __FILESINK_CONTROLL( "Ignore as key frame not yet provided!" )
            ignoreBuffer = ETrue;
            }
        }
    else if ( aBufferType == CCMRMediaBuffer::EVideoH264Bytestream )
        {
        if ( !iDecSpecInfoProvided )
            {
            __FILESINK_CONTROLL( "Ignore as dec spec info not yet provided!" )
            ignoreBuffer = ETrue;
            }
        }
    else
        {
        // NOP
        }
    
    __FILESINK_CONTROLL_INT1( "CMccFileSink::CheckWritingPermission, ignore:", 
                              ignoreBuffer )
    
    return ignoreBuffer;
    }

// ---------------------------------------------------------------------------
// CMccFileSink::SetStateL
// ---------------------------------------------------------------------------
//
TBool CMccFileSink::SetStateL( TFileSinkState aState )
    {
    TBool controlSink( iCurrentState != aState );
    TBool transitionOk( iCurrentState == aState );
    switch ( aState )
        {
        case EReady:
            { 
            transitionOk = ETrue;
            iCurrentState = aState;
            break;
            }
        case EPaused:
            {
            if ( iCurrentState == ERecording )
                {
                transitionOk = ETrue;
                iCurrentState = aState;
                }
            break;
            }
        case ERecording:
            {
            if ( iCurrentState == EReady || iCurrentState == EPaused )
                {
                transitionOk = ETrue;
                iCurrentState = aState;
                }
            break;
            }
        case EStopped:
            {
            // State is not changed to stopped if there's several users
            transitionOk = ETrue;
            const TInt KMccFileSinkMultipleUsers = 2;
            if ( iUsers.Count() < KMccFileSinkMultipleUsers )
                {
                iCurrentState = aState;
                }
            break;
            }
        default:
            {
            break;       
            }
        }

    __ASSERT_ALWAYS( transitionOk, User::Leave( KErrNotReady ) );
    
    return controlSink;    
    }

// -----------------------------------------------------------------------------
// CMccFileSink::DoSinkPrimeL
// -----------------------------------------------------------------------------
//
void CMccFileSink::DoSinkPrimeL( TBool aSendEvent )
	{
	__FILESINK_CONTROLL( "CMccFileSink::DoSinkPrimeL" )

    // If disk is full at beginning, ignore error at this stage and send disk
    // full event when first buffer is tried to be written to file.
    //
    if ( SetStateL( EReady ) )
        {
        HBufC8* codecType = GetCodecTypeStringLC( ActiveUserL().iCodecInfo );
    	TRAPD( err, iFileComposer->OpenFileL( iFileName, 
    	                                      iAudioFourCC, 
    	                                      *codecType ) );  

        CleanupStack::PopAndDestroy( codecType );
    	if ( err == KErrDiskFull )
    	    {
    	    __FILESINK_CONTROLL( "CMccFileSink::SinkPrimeL, disk full" )
    	    iNotifySizeLimitReached = ETrue;
    	    err = KErrNone;
    	    }
    	User::LeaveIfError( err );
    	
    	iDecSpecInfoProvided = EFalse;
        }

    if ( aSendEvent )
        {
    	SendStreamEventToClient( KMccStreamPrepared );	
        }
	}

// -----------------------------------------------------------------------------
// CMccFileSink::DoSinkPlayL
// -----------------------------------------------------------------------------
//
void CMccFileSink::DoSinkPlayL( TBool aSendEvent )
    { 
    __ASSERT_ALWAYS( !iSizeLimitReached, User::Leave( KErrDiskFull ) );
    
    TMccEventType eventType = 
        iCurrentState == EPaused ? KMccStreamResumed : KMccStreamStarted;
    TFileSinkState oldState = iCurrentState;
    
    if ( SetStateL( ERecording ) )
        {    
        // First frame written to file should be keyframe
        iKeyFrameProvided = EFalse;
        
    	if ( oldState == EPaused )
            {
            iT2.HomeTime();
            __FILESINK_CONTROLL_INT1( "CMccFileSink::SinkPlayL, iT2=", iT2.Int64() )
            SetPausedDuration( iT1, iT2 );
            }
        }
	
	if ( aSendEvent )
	    {
        SendStreamEventToClient( eventType );
	    }
    }

// -----------------------------------------------------------------------------
// CMccFileSink::DoSinkPauseL
// -----------------------------------------------------------------------------
//
void CMccFileSink::DoSinkPauseL( TBool aSendEvent )
    { 
    TFileSinkState oldState = iCurrentState;
    
    if ( SetStateL( EPaused ) )
        {  
        if ( oldState == ERecording )
            {
            iT1.HomeTime();  
            __FILESINK_CONTROLL_INT1( "CMccFileSink::SinkPauseL, iT1=", iT1.Int64() )
            }   
        }
    
	if ( aSendEvent )
	    {
	    SendStreamEventToClient( KMccStreamPaused );
	    }
    }
    
// -----------------------------------------------------------------------------
// CMccFileSink::DoSinkStopL
// -----------------------------------------------------------------------------
//
void CMccFileSink::DoSinkStopL( TBool aSendEvent )
    {
    __FILESINK_CONTROLL( "CMccFileSink::DoSinkStopL" )
    
     if ( SetStateL( EStopped ) )
        {  
    	iFileComposer->SinkStopL();
        
        ResetTimers();
        }

    if ( aSendEvent )
        {
	    SendStreamEventToClient( KMccStreamStopped );	
        }
    }	

// -----------------------------------------------------------------------------
// CMccFileSink::DoCodecChangeL
// -----------------------------------------------------------------------------
//
void CMccFileSink::DoCodecChangeL()
    {
    __FILESINK_CONTROLL( "CMccFileSink::DoCodecChangeL" )
    
    TFileSinkState oldState = iCurrentState;
    
    DoSinkStopL( EFalse );
    DoSinkPrimeL( EFalse );
    
    if ( oldState == ERecording || oldState == EPaused )
        {
        DoSinkPlayL( EFalse );
        if ( oldState == EPaused )
            {
            DoSinkPauseL( EFalse );
            }
        }
    
    __FILESINK_CONTROLL( "CMccFileSink::DoCodecChangeL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccFileSink::AddUserL()
// -----------------------------------------------------------------------------
// 
void CMccFileSink::AddUserL( MAsyncEventHandler* aEventHandler )
    {
    TMccFileSinkUser* user = new ( ELeave ) TMccFileSinkUser( aEventHandler );
    CleanupStack::PushL( user );
    iUsers.AppendL( user );
    CleanupStack::Pop( user );
    }

// -----------------------------------------------------------------------------
// CMccFileSink::ActiveUserL()
// -----------------------------------------------------------------------------
//
TMccFileSinkUser& CMccFileSink::ActiveUserL()
    {
    __ASSERT_ALWAYS( iUsers.Count() > 0, User::Leave( KErrNotReady ) );
    TInt index( 0 );
    if ( iActiveUserIndex != KErrNotFound && iActiveUserIndex < iUsers.Count() )
        {
        index = iActiveUserIndex;
        }
    return *iUsers[ index ];
    }

// -----------------------------------------------------------------------------
// CMccFileSink::IsActiveUser()
// -----------------------------------------------------------------------------
//
TBool CMccFileSink::IsActiveUser( MAsyncEventHandler* aUser )
    {
    TInt index = 
        MccUserArray<TMccFileSinkUser>::FindUserEntryIndexForCurrent( iUsers, aUser );
    return ( index != KErrNotFound && index == iActiveUserIndex );
    }

// -----------------------------------------------------------------------------
// CMccFileSink::UpdateActiveUserL()
// -----------------------------------------------------------------------------
//
TFourCC CMccFileSink::UpdateActiveUserL( TMediaId aMediaId, MDataSource& aDataProvider )
    {
    TBool doCodecChange( EFalse );
    
    TFourCC providerDataType = aDataProvider.SourceDataTypeCode( aMediaId );
    
    if ( iActiveUserIndex == KErrNotFound )
        {
        SetActiveUserIndex( 0 );
        doCodecChange = ETrue;
        
        __FILESINK_CONTROLL_INT1( 
            "CMccFileSink::UpdateActiveUserL, new active user index:", iActiveUserIndex )
        }
    else
        {
        if ( ActiveUserL().iCodecInfo.iFourCC != providerDataType )
            {    
            __FILESINK_CONTROLL_INT1( 
                "CMccFileSink::UpdateActiveUserL, provider fourcc:", 
                providerDataType.FourCC() )
            
            __FILESINK_CONTROLL_INT1( 
                "CMccFileSink::UpdateActiveUserL, active user fourcc:", 
                ActiveUserL().iCodecInfo.iFourCC.FourCC() )
            
            for ( TInt i = 0; i < iUsers.Count() && !doCodecChange; i++ )
                {
                if ( iUsers[ i ]->iCodecInfo.iFourCC == providerDataType )
                    {
                    SetActiveUserIndex( i );
                    doCodecChange = ETrue;
                    }
                }
            }
        }
        
    if ( doCodecChange )
        {
        SetCurrentUser( ActiveUserL().iEventHandler );
        DoCodecChangeL();
        }
        
    return providerDataType;
    }

// -----------------------------------------------------------------------------
// CMccFileSink::SetActiveUserIndex()
// -----------------------------------------------------------------------------
//
void CMccFileSink::SetActiveUserIndex( TInt aIndex )
    {
    __FILESINK_CONTROLL_INT1( "CMccFileSink::SetActiveUserIndex, index:", aIndex )
            
    iActiveUserIndex = aIndex;
    }
    
#ifndef EKA2
EXPORT_C TInt E32Dll( TDllReason )
    {
    return KErrNone;
    }
#endif

