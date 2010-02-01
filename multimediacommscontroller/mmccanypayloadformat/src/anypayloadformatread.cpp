/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
#include <e32base.h>
#include <mmf/common/mmffourcc.h>
#include <mmf/server/mmfdatabuffer.h>
#include "anypayloadformatread.h"
#include "mccrtpdatasource.h"
#include "mccinternaldef.h"
#include "mccanyformatlogs.h"

const TInt KNumValue4(4);
const TInt KNumValue16(16);


// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::CAnyPayloadFormatRead
//
// CAnyPayloadFormatRead default constructor, can NOT contain any code,
// that might leave
// Phase #1 of 2-phase constructor
// -----------------------------------------------------------------------------
//
CAnyPayloadFormatRead::CAnyPayloadFormatRead ( )
    {
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAnyPayloadFormatRead::ConstructL ( MDataSource* aSource )
    {
    __ANYFORMAT_CONTROLL( "CAnyPayloadFormatRead::ConstructL" )
    __ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) );
 
    iRtpDataSource = aSource;
    iClip = aSource;
	iFillRequested = EFalse;	
	iBufferToReadExists = EFalse;
    
    // Initialize decoding state machine
    iStateMachine = CFormatDecodeStateMachine::NewL( this );
    iStateMachine->ChangeState( EDecodeIdle );
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::NewL
//
// Two-phased constructor.
// Static function for creating and constructing an instance of 
// the AMR-NB format.
//
// Returns:  CAnyPayloadFormatRead* : pointer to created instance
// -----------------------------------------------------------------------------
//
CAnyPayloadFormatRead* CAnyPayloadFormatRead::NewL( 
	MDataSource* aSource )
    {
    __ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) );

    __ASSERT_ALWAYS( ( KMccRtpSourceUid == aSource->DataSourceType() ||
                       KMccVideoSourceUid == aSource->DataSourceType() ||
                       KMccMultiplexerUid == aSource->DataSourceType() ), 
                     User::Leave( KErrArgument ) );
	CAnyPayloadFormatRead* self =
		new ( ELeave ) CAnyPayloadFormatRead;
	CleanupStack::PushL ( self );
    self->ConstructL ( aSource );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::~CAnyPayloadFormatRead
//
// Destructor.
// -----------------------------------------------------------------------------
//
CAnyPayloadFormatRead::~CAnyPayloadFormatRead( )
    {
    delete iFrameBufferOne;
    delete iFrameBufferTwo;
    if ( iSourceBufOwnership )
        {
        delete iPayloadBuffer;
        }
    else
        {
        iPayloadBuffer = NULL;
        }
    
    if ( iStateMachine )
        {
        iStateMachine->Cancel( );
        delete iStateMachine;
        }
    iCurrentBuffer = NULL;
    iRtpDataSource = NULL;
    iClip = NULL;
    iDataPath = NULL;
    iEventHandler = NULL;
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::Streams
// Return number of audio streams for the given media
// -----------------------------------------------------------------------------
//
TUint CAnyPayloadFormatRead::Streams( TUid /*aMediaType*/ ) const
    {
    return 1;
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::FrameTimeInterval
// Return the frame time interval for the given media
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CAnyPayloadFormatRead::FrameTimeInterval( 
    TMediaId /*aMediaId*/ ) const
    {
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::FillBufferL
// Fill Buffer
// -----------------------------------------------------------------------------
//
void CAnyPayloadFormatRead::FillBufferL( CMMFBuffer* aBuffer, 
                                         MDataSink*  aConsumer, 
                                         TMediaId    aMediaId )
    {
    __ANYFORMAT_MEDIA( "CAnyPayloadFormatRead::FillBufferL" )
    
    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aBuffer->Type() == KUidMmfDataBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aBuffer == iCurrentBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aConsumer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iStateMachine, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iFrameBufferOne, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iFrameBufferTwo, User::Leave( KErrArgument ) );

    iDataPath = aConsumer;
    iMediaId = aMediaId;
    iFillRequested = ETrue;

	if ( iCurrentBuffer )
		{
		iCurrentBuffer->SetStatus( EAvailable );
		iCurrentBuffer = NULL;		
		}

	if (iFrameBufferOne->Status() == EFull )
		{
		iCurrentBuffer = iFrameBufferOne;
        iStateMachine->ChangeState( ESourceDataReady );				
		}
	else if (iFrameBufferTwo->Status() == EFull )
		{
		iCurrentBuffer = iFrameBufferTwo;
        iStateMachine->ChangeState( ESourceDataReady );		
		}
    else
        {
        FillSourceBufferL();
        } 
    };

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::FillSinkBuffer
// Read RTP payload and convert it into AMR frames.
// -----------------------------------------------------------------------------
//
void CAnyPayloadFormatRead::FillSinkBufferL( )
    {
    __ANYFORMAT_MEDIA( "CAnyPayloadFormatRead::FillSinkBuffer" )
   	__ASSERT_ALWAYS( iStateMachine, User::Leave( KErrArgument ) );
   	__ASSERT_ALWAYS( iCurrentBuffer, User::Leave( KErrArgument ) );

    iStateMachine->ChangeState( EEmptyDataToSink ); 
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::FillSourceBufferL
// Send fill buffer request to RTP Data Soure
// -----------------------------------------------------------------------------
//
void CAnyPayloadFormatRead::FillSourceBufferL( )
    { 
    __ASSERT_ALWAYS( iClip, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iPayloadBuffer, User::Leave( KErrArgument ) );

    iClip->FillBufferL(iPayloadBuffer, this, iMediaId );     
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::SendDataToSinkL
// Send full frame buffer to Data Path
// -----------------------------------------------------------------------------
//
void CAnyPayloadFormatRead::SendDataToSinkL( )
    {
    __ANYFORMAT_MEDIA( "CAnyPayloadFormatRead::SendDataToSinkL" )
   	__ASSERT_ALWAYS( iDataPath, User::Leave( KErrArgument ) );
   	__ASSERT_ALWAYS( iCurrentBuffer, User::Leave( KErrArgument ) );
	
    iDataPath->BufferFilledL( iCurrentBuffer );
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::CreateSourceBufferL
// Create a source buffer for the given media and indicate in aReference if 
// buffer is created.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CAnyPayloadFormatRead::CreateSourceBufferL( 
    TMediaId /*aMediaId*/, 
    TBool &aReference )
    {
    __ANYFORMAT_MEDIA( "CAnyPayloadFormatRead::CreateSourceBufferL" )
    // the source buffers belong to AmrPayloadFormatRead not to datapath
    // aference should be set to ETrue and destried by AmrPayloadFormatRead 
    // itself.
    aReference = ETrue;
    iCurrentBuffer = iFrameBufferOne;
    return iFrameBufferOne; 
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::CreateSourceBufferL
// Create a source buffer for the given media, setting frame size to match
// the given sink buffer
// -----------------------------------------------------------------------------
//
CMMFBuffer* CAnyPayloadFormatRead::CreateSourceBufferL(
    TMediaId aMediaId,
    CMMFBuffer& /*aSinkBuffer*/, 
    TBool &aReference )
    {
    __ANYFORMAT_MEDIA( "CAnyPayloadFormatRead::CreateSourceBufferL" )
    return CreateSourceBufferL( aMediaId, aReference );
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::SourceDataTypeCode
// Return the source data type ( four CC code ) for the given media
// -----------------------------------------------------------------------------
//
TFourCC CAnyPayloadFormatRead::SourceDataTypeCode( 
	TMediaId /*aMediaId*/ )
    {
    return iFourCC;
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::SetSourceDataTypeCode
// Set the source data type to the given four CC code for the given 
// media
// -----------------------------------------------------------------------------
//
TInt CAnyPayloadFormatRead::SetSourceDataTypeCode( TFourCC aSourceFourCC, 
    											   TMediaId aMediaId )
    {
    __ANYFORMAT_CONTROLL( "CAnyPayloadFormatRead::SetSourceDataTypeCode" )
    
    if( iClip )
	    {
		iFourCC = aSourceFourCC;
		iMediaId = aMediaId;

		iClip->SetSourceDataTypeCode( iFourCC, iMediaId );

		return KErrNone;	
	    }
	else
		{
		return KErrArgument;	
		}
   
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::SourceThreadLogon
// Log in to the source thread
// -----------------------------------------------------------------------------
//
TInt CAnyPayloadFormatRead::SourceThreadLogon( 
    MAsyncEventHandler& aEventHandler )
    {
    __ANYFORMAT_CONTROLL( "CAnyPayloadFormatRead::SourceThreadLogon" )
    if ( iClip )
	    {
	    iEventHandler = &aEventHandler;
	    iClip->SourceThreadLogon( aEventHandler );
	    return KErrNone;	
	    }
	else
		{
		return KErrArgument;	
		}   
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::NegotiateSourceL( MDataSink& aDataSink )
// Negotiate source settings to match data sink object.
// Re-size frame buffers if needed
// -----------------------------------------------------------------------------
//
void CAnyPayloadFormatRead::NegotiateSourceL( MDataSink& /*aDataSink*/ )
    {
    __ANYFORMAT_CONTROLL( "CAnyPayloadFormatRead::NegotiateSourceL" )
    __ASSERT_ALWAYS( iClip, User::Leave( KErrArgument ) );
    iClip->NegotiateSourceL( *this );
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::SourceThreadLogoff
// Log out of the source thread.
// -----------------------------------------------------------------------------
//
void CAnyPayloadFormatRead::SourceThreadLogoff( )
    {
    __ANYFORMAT_CONTROLL( "CAnyPayloadFormatRead::SourceThreadLogoff" )
    if ( iClip )
	    {
	    iClip->SourceThreadLogoff( );
	    }
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::DataBufferFilledL
// -----------------------------------------------------------------------------
//
void CAnyPayloadFormatRead::DataBufferFilledL( CMMFBuffer* aBuffer, 
                                           const TRtpRecvHeader& aRtpHeader )
    {
    __ANYFORMAT_MEDIA( "CAnyPayloadFormatRead::DataBufferFilledL" )
    __ANYFORMAT_MEDIA_INT1( "CAnyPayloadFormatRead::DataBufferFilledL iPadding:", aRtpHeader.iPadding )
    __ANYFORMAT_MEDIA_INT1( "CAnyPayloadFormatRead::DataBufferFilledL iExtension:", aRtpHeader.iExtension )
    __ANYFORMAT_MEDIA_INT1( "CAnyPayloadFormatRead::DataBufferFilledL iCsrcCount:", aRtpHeader.iCsrcCount )
    __ANYFORMAT_MEDIA_INT1( "CAnyPayloadFormatRead::DataBufferFilledL iMarker:", aRtpHeader.iMarker )
    __ANYFORMAT_MEDIA_INT1( "CAnyPayloadFormatRead::DataBufferFilledL iPayloadType:", aRtpHeader.iPayloadType )
    __ANYFORMAT_MEDIA_INT1( "CAnyPayloadFormatRead::DataBufferFilledL iSeqNum:", aRtpHeader.iSeqNum )
    __ANYFORMAT_MEDIA_INT1( "CAnyPayloadFormatRead::DataBufferFilledL iTimestamp:", aRtpHeader.iTimestamp );
    __ANYFORMAT_MEDIA_INT1( "CAnyPayloadFormatRead::DataBufferFilledL iHeaderExtension:", (TInt)aRtpHeader.iHeaderExtension )
    __ANYFORMAT_MEDIA_INT1( "CAnyPayloadFormatRead::DataBufferFilledL iCsrcList:", (TInt)aRtpHeader.iCsrcList )

    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    
    aBuffer->SetTimeToPlay( aRtpHeader.iTimestamp );
    aBuffer->SetLastBuffer( aRtpHeader.iMarker );
	aBuffer->SetFrameNumber( aRtpHeader.iSeqNum );
		
    BufferFilledL( aBuffer );
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::BufferFilledL
// Called after the data buffer is filled. Update the number of bytes read
// and the current read position for the next read operation.
// -----------------------------------------------------------------------------
//
void CAnyPayloadFormatRead::BufferFilledL( CMMFBuffer* aBuffer )
	{
    __ANYFORMAT_MEDIA( "CAnyPayloadFormatRead::BufferFilledL2" )
    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aBuffer->Type() == KUidMmfDataBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iPayloadBuffer == aBuffer , User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iClip, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iStateMachine, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iFrameBufferOne , User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iFrameBufferTwo , User::Leave( KErrArgument ) );

    iPacketCount++;
    
    CMMFDataBuffer* dataBuffer = static_cast<CMMFDataBuffer*>( aBuffer );
    
    if ( dataBuffer->Data().Length() == 0 )
        {
        __ANYFORMAT_MEDIA( "CAnyPayloadFormatRead::BufferFilledL2, zero length payload" )
        
        // Drop buffer and ask for more
        FillSourceBufferL();
        }
    else if ( iFrameBufferOne->Status() == EAvailable )
        {
	    iFrameBufferOne->Data().Copy( dataBuffer->Data() );
	    iFrameBufferOne->SetTimeToPlay( aBuffer->TimeToPlay() );
		iFrameBufferOne->SetLastBuffer( aBuffer->LastBuffer() );
		iFrameBufferOne->SetFrameNumber( aBuffer->FrameNumber() );

		iFrameBufferOne->SetStatus( EFull );

        if (iFrameBufferTwo->Status() == EAvailable )
        	{
        	iCurrentBuffer = iFrameBufferOne;
        	iStateMachine->ChangeState( ESourceDataReady );
        	}
        }
    else if ( iFrameBufferTwo->Status() == EAvailable )
        {
	    iFrameBufferTwo->Data().Copy( dataBuffer->Data() );
        iFrameBufferTwo->SetTimeToPlay( aBuffer->TimeToPlay() );
		iFrameBufferTwo->SetLastBuffer( aBuffer->LastBuffer() );
		iFrameBufferTwo->SetFrameNumber( aBuffer->FrameNumber() );
		iFrameBufferTwo->SetStatus( EFull );
		
        if (iFrameBufferOne->Status() == EAvailable )
	        {
	        iCurrentBuffer = iFrameBufferTwo;   
	       	iStateMachine->ChangeState( ESourceDataReady );    	
	        }
       	}
    else
        {
        // Neither of the buffers is available
        User::Leave( KErrNotReady );
        }
	}
	
// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::Duration
// Return the clip duration for the given media.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CAnyPayloadFormatRead::Duration( 
	TMediaId /*aMediaType*/ ) const
    {
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::LastDlPacketCount
// Return current packet count for current talk spurt
// -----------------------------------------------------------------------------
//
TInt CAnyPayloadFormatRead::LastDlPacketCount( )
    {
    return iPacketCount;
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::CancelDlRequest( )
// 
// -----------------------------------------------------------------------------
//
void CAnyPayloadFormatRead::CancelDlRequest()
    {
    __ANYFORMAT_CONTROLL( "CAnyPayloadFormatRead::CancelDlRequest" )
 
    if( iStateMachine && iFrameBufferOne && iFrameBufferTwo )
	    {
		iStateMachine->Cancel();
	    iStateMachine->ChangeState( EDecodeIdle );
	    
	    iFrameBufferOne->SetLastBuffer( EFalse );
	    iFrameBufferTwo->SetLastBuffer( EFalse );

	    iFrameBufferOne->SetStatus( EAvailable );
	    iFrameBufferTwo->SetStatus( EAvailable );

	    iBufferToReadExists = EFalse;
	    iFillRequested = EFalse;	
	    }
 
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::SourcePrimeL()
// 
// -----------------------------------------------------------------------------
//
void CAnyPayloadFormatRead::SourcePrimeL()
    {
    __ANYFORMAT_CONTROLL( "CAnyPayloadFormatRead::SourcePrimeL" )
    __ASSERT_ALWAYS( iClip, User::Leave( KErrArgument ) );
    iClip->SourcePrimeL();
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::SourcePlayL()
//
// -----------------------------------------------------------------------------
//
void CAnyPayloadFormatRead::SourcePlayL()
    {
    __ANYFORMAT_CONTROLL( "CAnyPayloadFormatRead::SourcePlayL" )
    __ASSERT_ALWAYS( iClip, User::Leave( KErrArgument ) );

    iPacketCount = 0;    
    iClip->SourcePlayL();
    };

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::SourcePauseL()
//
// -----------------------------------------------------------------------------
//
void CAnyPayloadFormatRead::SourcePauseL()
    {
    __ANYFORMAT_CONTROLL( "CAnyPayloadFormatRead::SourcePauseL" )
    __ASSERT_ALWAYS( iClip, User::Leave( KErrArgument ) );
    this->CancelDlRequest();
    iClip->SourcePauseL();
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::SourceStopL( )
//
// -----------------------------------------------------------------------------
//
void CAnyPayloadFormatRead::SourceStopL()
    {
    __ANYFORMAT_CONTROLL( "CAnyPayloadFormatRead::SourceStopL" )
    __ASSERT_ALWAYS( iClip, User::Leave( KErrArgument ) );
    this->CancelDlRequest();
    iClip->SourceStopL();
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::SinkDataTypeCode()
// Returns the datatype code for this Format Decoder
// -----------------------------------------------------------------------------
//
TFourCC CAnyPayloadFormatRead::SinkDataTypeCode( TMediaId /*aMediaId*/ )
    {
    return iFourCC;
    }
   
// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::ConfigurePayloadFormatL
// Configure payload decoding parameters.
// -----------------------------------------------------------------------------
//
void CAnyPayloadFormatRead::ConfigurePayloadFormatL( 
	const TDesC8& aConfigParams )
    {
    __ANYFORMAT_CONTROLL( "CAnyPayloadFormatRead::ConfigurePayloadFormatL" )
    
    TMccCodecInfoBuffer infoBuffer;
    infoBuffer.Copy( aConfigParams );
    TMccCodecInfo cInfo = infoBuffer();
    
    if ( cInfo.iIsUpdate && cInfo.iFrameSize == iCInfo.iFrameSize )
        {
         __ANYFORMAT_CONTROLL( "CAnyPayloadFormatRead::ConfigurePayloadFormatL, nothing to configure" )
        return;
        } 
    
    TUint framesize;
    if ( iMediaId.iMediaType == KUidMediaTypeVideo ) // for video
        {
        __ANYFORMAT_CONTROLL( "CAnyPayloadFormatRead::ConfigurePayloadFormatL, \
iMediaId.iMediaType == KUidMediaTypeVideo" )

        framesize = cInfo.iFrameSize * KNumValue4; 
        }
    else // for audio
        {
        if ( !cInfo.iMaxPtime )
            {
            framesize = cInfo.iFrameSize * KNumValue16;   
            }
        else
            {
            if( !cInfo.iHwFrameTime ) 
                {
                User::Leave( KErrArgument );
                }
            TUint framesPerPackets = cInfo.iMaxPtime / cInfo.iHwFrameTime;
            framesize = cInfo.iFrameSize * framesPerPackets;    
            }
        }
    
    __ANYFORMAT_CONTROLL_INT1( "CAnyPayloadFormatRead, buffer size", framesize )
  	if ( !iFrameBufferOne )
	    {
	    iFrameBufferOne = 
	    	CMMFDataBuffer::NewL( framesize );
	    iFrameBufferOne->SetStatus( EAvailable );
	    }
	    
	if ( !iFrameBufferTwo )
	    {
	    iFrameBufferTwo =
	    	CMMFDataBuffer::NewL( framesize );
	    iFrameBufferTwo->SetStatus( EAvailable );
	    }
	    
	if ( iPayloadBuffer && iSourceBufOwnership )	
	    {
	    delete iPayloadBuffer;
	    iPayloadBuffer = NULL;
	    }
    
    iPayloadBuffer 
        = CreateClipBufferL( framesize, iSourceBufOwnership );
    
    iCurrentBuffer = NULL;
    
    iCInfo = cInfo;
    }

// -----------------------------------------------------------------------------
// CAnyPayloadFormatRead::CreateClipBufferL
// Creates buffer needed in data transfer with format readers clip.
// -----------------------------------------------------------------------------
//
CMMFDataBuffer* CAnyPayloadFormatRead::CreateClipBufferL( 
        TUint aSize, TBool& aIsOwnBuffer )
    {
    __ANYFORMAT_CONTROLL( "CAnyPayloadFormatRead::CreateClipBufferL" );
    
    CMMFDataBuffer* buffer( NULL );
    
    if ( iClip->CanCreateSourceBuffer() )
        {
        static_cast<CMMFFormatDecode*>( iClip )->SuggestSourceBufferSize( aSize );
        
        TBool reference( EFalse );
        CMMFBuffer* sourceBuf 
            = iClip->CreateSourceBufferL( KUidMediaTypeAudio, reference );
        TBool isSupportedBuf 
            = CMMFBuffer::IsSupportedDataBuffer( sourceBuf->Type() );
        TBool isOwnBuffer = reference ? EFalse : ETrue;
        
        if ( !isSupportedBuf )
            {
            if ( isOwnBuffer )
                {
                delete sourceBuf;
                }
            
            User::Leave( KErrNotSupported );
            }
        
        aIsOwnBuffer = isOwnBuffer;
        buffer = static_cast<CMMFDataBuffer*>( sourceBuf );
        }
    else
        {
        aIsOwnBuffer = ETrue;
        buffer = CMMFDataBuffer::NewL( aSize );
        }
    
    return buffer;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
