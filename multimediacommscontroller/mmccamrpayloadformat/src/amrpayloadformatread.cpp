/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    PayloadFormat plugin capable to read RTP payload containing
*                AMR audio.
*
*/




// INCLUDE FILES
#include <e32base.h>
#include <mmf/common/mmffourcc.h>
#include "amrpayloadformatread.h"
#include "amrpayloadencoder.h"
#include "amrpayloaddecoder.h"
#include "amrpayloaddecoderoa.h"
#include "amrpayloadformatutil.h"
#include "amrcommonutil.h"
#include "mccrtpdatasource.h"
#include "mccuids.hrh"
#include "mccinternaldef.h"
#include "amrpayloadformatter.h"
#include "mccredpayloadread.h"

#ifdef VOIP_TRACE_ENABLED
#include <voip_trace.h>
#endif

// CONSTANTS
const TReal KTimeMultiplier = 0.001;

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::CAmrPayloadFormatRead
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAmrPayloadFormatRead::CAmrPayloadFormatRead ( ) :
    iIsNb( ETrue ), iSamplingRate( KAmrNbSampleRate ), iChannels( 1 ),
    iMode( EAmrFrame12_2 ), iCmr ( EAmrModeReq7 ), iStreamEncoder( TStreamEncoder() )
    {
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::ConstructL ( MDataSource* aSource )
    {
    User::LeaveIfNull( aSource );

    iRtpDataSource = aSource;
    iClip = aSource;
    iBufferToReadExists = EFalse;

    iFourCC.Set( TFourCC( ' ','A','M','R' ) );

    // Set default value of iChannels and iFramesPerPacket
    iFramesPerPacket = 1;
    iChannels = 1;
    
    // Initialize decoding state machine
    iStateMachine = CFormatDecodeStateMachine::NewL( this );
    iStateMachine->ChangeState( EDecodeIdle );

    iCurrentBuffer = EBufferOne;
    
    #ifdef FTD_ENABLED
    
    User::LeaveIfError( iJBufStatsQueue.OpenGlobal( KMccJBufferStats, 
        EOwnerProcess ) );    
    
    #endif        
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::NewL
//
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAmrPayloadFormatRead* CAmrPayloadFormatRead::NewL ( MDataSource* aSource )
    {
    __ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) );

    AMR_PAYLOAD_FORMAT_READ ( "CAmrPayloadFormatRead::NewL" );
    
    CAmrPayloadFormatRead* self = new ( ELeave ) CAmrPayloadFormatRead;
    CleanupStack::PushL ( self );
    self->ConstructL ( aSource );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::~CAmrPayloadFormatRead
//
// Destructor.
// -----------------------------------------------------------------------------
//
CAmrPayloadFormatRead::~CAmrPayloadFormatRead ( )
    {
    // commented out CreateSourceBufferL now sets aReference to ETrue.
    // datapath doesn't clean this buffer any longer.
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
    	AMR_PAYLOAD_FORMAT_READ2 ( "CAmrPayloadFormatRead::~CAmrPayloadFormatRead: 0x%x", this );
    	AMR_PAYLOAD_FORMAT_READ2 ( "CAmrPayloadFormatRead::~CAmrPayloadFormatRead iFrameBufferOne: 0x%x", iFrameBufferOne );
    	AMR_PAYLOAD_FORMAT_READ2 ( "CAmrPayloadFormatRead::~CAmrPayloadFormatRead iFrameBufferTwo: 0x%x", iFrameBufferTwo );
    	AMR_PAYLOAD_FORMAT_READ2 ( "CAmrPayloadFormatRead::~CAmrPayloadFormatRead iSourceBuffer: 0x%x", iSourceBuffer );
    	AMR_PAYLOAD_FORMAT_READ2 ( "CAmrPayloadFormatRead::~CAmrPayloadFormatRead iStateMachine: 0x%x", iStateMachine );
    	AMR_PAYLOAD_FORMAT_READ2 ( "CAmrPayloadFormatRead::~CAmrPayloadFormatRead iPayloadDecoder: 0x%x", iPayloadDecoder );
    	AMR_PAYLOAD_FORMAT_READ2 ( "CAmrPayloadFormatRead::~CAmrPayloadFormatRead iRtpDataSource: 0x%x", iRtpDataSource );
    	AMR_PAYLOAD_FORMAT_READ2 ( "CAmrPayloadFormatRead::~CAmrPayloadFormatRead iClip: 0x%x", iClip );
    	AMR_PAYLOAD_FORMAT_READ2 ( "CAmrPayloadFormatRead::~CAmrPayloadFormatRead iDataPath: 0x%x", iDataPath );
    	AMR_PAYLOAD_FORMAT_READ2 ( "CAmrPayloadFormatRead::~CAmrPayloadFormatRead iBufferToRead: 0x%x", iBufferToRead );
    #endif
    

    delete iFrameBufferOne;
    delete iFrameBufferTwo;
    if ( iSourceBufOwnership )
        {
        delete iSourceBuffer;
        }
    iSourceBuffer = NULL;

    if ( iStateMachine )
        {
        iStateMachine->Cancel( );
        delete iStateMachine;
        }
    
    delete iPayloadDecoder;
    iRtpDataSource = NULL;
    iClip = NULL;
    iDataPath = NULL;
    iBufferToRead = NULL;
    iEventHandler = NULL;

    #ifdef FTD_ENABLED
    iJBufStatsQueue.Close();
    #endif
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::Streams
// Return number of audio streams for the given media
// -----------------------------------------------------------------------------
//
TUint CAmrPayloadFormatRead::Streams( TUid aMediaType ) const
    {
    if ( KUidMediaTypeAudio == aMediaType )
        {
        return 1;
        }
    else
        {
        return 0;
        }
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::FrameTimeInterval
// Return the frame time interval for the given media
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CAmrPayloadFormatRead::FrameTimeInterval( 
        TMediaId aMediaId ) const
    {
    if ( KUidMediaTypeAudio == aMediaId.iMediaType )
        {
        TInt hwFrametime = static_cast<TInt>( iCInfo.iHwFrameTime );
        return TTimeIntervalMicroSeconds( TInt64( hwFrametime ) );
        }
    else
        {
        return TTimeIntervalMicroSeconds( TInt64( 0 ) );
        }
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::FillBufferL
// Fill Buffer
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::FillBufferL( CMMFBuffer* aBuffer, 
        MDataSink* aConsumer, TMediaId aMediaId )
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
    	AMR_PAYLOAD_FORMAT_READ ( "CAmrPayloadFormatRead::FillBufferL( )" );
    #endif
    
    User::LeaveIfNull( aBuffer );
    User::LeaveIfNull( aConsumer );

    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
    	AMR_PAYLOAD_FORMAT_READ3 ( "CAmrPayloadFormatRead::FillBufferL( ) buffer 0x%x passed in with length %d bytes \n", aBuffer, aBuffer->BufferSize( ) );
    #endif
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %x %x", MCC_TRACE, MCC_AMR_PLF_READ_FILLBUFFERL,
            aBuffer, aConsumer );
    #endif
    
    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        User::Leave( KErrNotSupported );
        }

    if ( KUidMmfDataBuffer != aBuffer->Type( ) )
        {
        User::Leave( KErrNotSupported );
        }

    iDataPath = aConsumer;
    iMediaId = aMediaId;

    // aBuffer is a reference to those frame buffers that AmrPayloadFormatRead owns
    aBuffer->SetLastBuffer( EFalse );

    iFillRequested = ETrue;

    if ( iCurrentBuffer == EBufferOne )
        {
        iFrameBufferTwo->SetStatus( EAvailable );
        }
    else
        {
        iFrameBufferOne->SetStatus( EAvailable );
        }

    if ( iBufferToReadExists ) // Payload has some frames not decoded
        {
        iStateMachine->ChangeState( ESourceDataReady );
        }
    else
        {
        #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ 
        	AMR_PAYLOAD_FORMAT_READ ( "CAmrPayloadFormatRead::FillBufferL( ), Request data from source\n" );
        #endif

        FillSourceBufferL(); // No payload ask for it
        } 
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::DoRead
// Reads data starting from the given position into the source buffer 
// -----------------------------------------------------------------------------
//
TBool CAmrPayloadFormatRead::DoRead( CMMFBuffer* aBuffer )
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        AMR_PAYLOAD_FORMAT_READ ( "CAmrPayloadFormatRead::DoRead" );
    #endif
    #ifdef VOIP_MCC_DL_TRACE_ENABLED
        VoipTrace( "%x %x %x", MCC_TRACE, MCC_AMR_PLF_READ_DOREAD, aBuffer );
    #endif
    
    if ( !aBuffer )
        {
        return EFalse;
        }

    if ( EFull == aBuffer->Status( ) )
        {
        #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        RDebug::Print ( _L ( "CAmrPayloadFormatRead::DoRead - Buffer already full" ) );
        #endif

        return EFalse;
        }

    if ( iNumOfFrames == 0 )
        {
        return EFalse;
        }
        
    TAmrFrame* frames = iPayloadDecoder->Frames( );
    TDes8& bufferDes = static_cast<CMMFDataBuffer*>( aBuffer )->Data();
    TInt byteIndex = bufferDes.Length( );
    
    TUint8* desPtr = const_cast<TUint8*>( bufferDes.Ptr( ) );
    iStreamEncoder.Initialize( desPtr, byteIndex, 0 );

   	#ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
    	AMR_PAYLOAD_FORMAT_READ2 ( "CAmrPayloadFormatRead::DoRead - iCurrentFrame = %d",
    	iCurrentFrame );
    #endif
    
    TInt bitCount = iPayloadDecoder->SpeechBitCount( frames[iCurrentFrame].iFrameType );
    // Calculate byte count
    TInt nextFrameLen =
    	bitCount % TInt(KNumValue8) == 0 ? bitCount / TInt(KNumValue8) : bitCount / TInt(KNumValue8) + 1;
    
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
    	AMR_PAYLOAD_FORMAT_READ4 ( "CAmrPayloadFormatRead::DoRead - iCurrentFrame = %d iNumOfFrames = %d byteIndex = %d",
        	iCurrentFrame, iNumOfFrames, byteIndex );
    #endif

    // Construct 8-bit frame header.
    // +-+-+-+-+-+-+-+-+
    // |P|  FT   |Q|P|P|
    // +-+-+-+-+-+-+-+-+
    iStreamEncoder.Encode( 0, 1 );
    iStreamEncoder.Encode( TInt( frames[iCurrentFrame].iFrameType ), KNumValue4 );
    iStreamEncoder.Encode( frames[iCurrentFrame].iFrameQualityInd, 1 );
    iStreamEncoder.Encode( 0, KNumValue2 );
    
    if ( bitCount > 0 )
        {
        // Construct core frame part ( speech data )
        iStreamEncoder.Encode( frames[iCurrentFrame].iFrameData, 0, 
                               frames[iCurrentFrame].iBitIndex, bitCount );
        if ( iStreamEncoder.BitIndex( ) != 0 )
            {
            // Need padding to byte boundray
            iStreamEncoder.Encode( 0, KNumValue8 - iStreamEncoder.BitIndex( ) );
            }
        }

    // Update iCurrentFrame
    iCurrentFrame++;

    byteIndex = byteIndex + nextFrameLen + 1;       // Add 1 for frame header byte

    // Get next frame length
    if ( iCurrentFrame < iNumOfFrames )
        {
        bitCount = iPayloadDecoder->SpeechBitCount( frames[iCurrentFrame].iFrameType );
        // Calculate byte count
        nextFrameLen =
        	bitCount % (TInt) KNumValue8 == 0 ? bitCount / (TInt) KNumValue8 : bitCount / (TInt) KNumValue8 + 1;
        }

    bufferDes.SetLength( byteIndex );
    
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
    	AMR_PAYLOAD_FORMAT_READ4 ( "CAmrPayloadFormatRead::DoRead - after decoding iCurrentFrame = %d iNumOfFrames = %d byteIndex = %d",
        iCurrentFrame, iNumOfFrames, byteIndex );
    #endif

    if ( iCurrentFrame == iNumOfFrames )
        {
        // Packet is fully decoded.
        // If it is the last buffer from RtpSourceSink, then we are not
        // expecting more packets. Also SetLastBuffer to notify datapath.
        if( iBufferToRead->LastBuffer( ) )
            {
            aBuffer->SetLastBuffer( ETrue );
            aBuffer->SetStatus( EFull );
            }
        return ETrue;
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::FillSinkBufferL
// Read RTP payload and convert it into AMR frames.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::FillSinkBufferL( )
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        AMR_PAYLOAD_FORMAT_READ ( "CAmrPayloadFormatRead::FillSinkBuffer( )" );
    #endif
    #ifdef VOIP_MCC_DL_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_AMR_PLF_READ_FILLSINKBUFFER );
    #endif    

    CMMFBuffer* curBuffer;
    if ( EBufferOne == iCurrentBuffer )
        {
        curBuffer = iFrameBufferOne;
        
        #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        AMR_PAYLOAD_FORMAT_READ ( "CAmrPayloadFormatRead::FillSinkBuffer( ) - Current Buffer One" );
        #endif
        }
    else
        {
        curBuffer = iFrameBufferTwo;
        
        #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        AMR_PAYLOAD_FORMAT_READ ( "CAmrPayloadFormatRead::FillSinkBuffer( ) - Current Buffer Two" );
        #endif
        }

	curBuffer->SetStatus( EAvailable );
    TBool allFrmBuffered = DoRead( curBuffer );
	curBuffer->SetFrameNumber(
		iRecvHeader.iTimestamp + ( ( iCurrentFrame - 1 )
	    * TUint( iCInfo.iHwFrameTime * iSamplingRate * KTimeMultiplier ) ) );

    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
    AMR_PAYLOAD_FORMAT_READ2 ( "CAmrPayloadFormatRead::FillSinkBuffer - FRAMENUM: %u",
        curBuffer->FrameNumber() );
    #endif	                           

    // Do not wait for the buffer to be filled up. If we do, this will cause jitters in
    // comfort-noise ( SID frame ) playing since SID-frame packets take considerably less
    // data bytes and it will delay waiting for many SID-frame packets before forwarding them to 
    // DataPath for further play-out processing.
    // Instead, forward the data as soon as we receive and decode one packet
    // to avoid jitters in play-out.
    if ( allFrmBuffered )
        {   
        iBufferToReadExists = EFalse;
        }


    // Do not forward zero length buffers
    if ( curBuffer->BufferSize() > 0 )
        {
        iStateMachine->ChangeState( EEmptyDataToSink );
        }
    else
        {
        #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ 
        	AMR_PAYLOAD_FORMAT_READ ( "CAmrPayloadFormatRead::FillSinkBufferL( ), Request data from source\n" );
        #endif
        
        FillSourceBufferL(); // No payload ask for it                           
        }
 
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
   	AMR_PAYLOAD_FORMAT_READ ( "CAmrPayloadFormatRead::FillSinkBuffer - DONE" );
    #endif
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::FillSourceBufferL
// Send fill buffer request to RTP Data Source
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::FillSourceBufferL( )
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        AMR_PAYLOAD_FORMAT_READ ( "CAmrPayloadFormatRead::FillSourceBufferL( )" );
    #endif 
    
    iClip->FillBufferL(iSourceBuffer, this, iMediaId );     
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SendDataToSinkL
// Send full frame buffer to Data Path
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::SendDataToSinkL( )
    {
    #ifdef VOIP_MCC_DL_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_AMR_PLF_READ_SENDDATATOSINKL );
    #endif

    iFillRequested = EFalse;
          
    if ( EBufferOne == iCurrentBuffer )
        {
        #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        AMR_PAYLOAD_FORMAT_READ ( "CAmrPayloadFormatRead::SendDataToSinkL( ) - BufferOne"  );
        #endif

        iDataPath->BufferFilledL( iFrameBufferOne );
        iCurrentBuffer = EBufferTwo;
        if ( iBufferToReadExists && !iFrameBufferOne->LastBuffer( ) )
            {
            // More payload buffer is needed
            iStateMachine->ChangeState( ESourceDataReady );
            }
        }
    else
        {
        #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        AMR_PAYLOAD_FORMAT_READ ( "CAmrPayloadFormatRead::SendDataToSinkL( ) - BufferTwo" );
        #endif

        iDataPath->BufferFilledL( iFrameBufferTwo );
        iCurrentBuffer = EBufferOne;
        if ( iBufferToReadExists && !iFrameBufferTwo->LastBuffer( ) )
            {
            // More payload buffer is needed
            iStateMachine->ChangeState( ESourceDataReady );
            }
       }

    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
    AMR_PAYLOAD_FORMAT_READ ( "CAmrPayloadFormatRead::SendDataToSinkL( ) - DONE" );
    #endif
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::CreateSourceBufferL
// Create a source buffer for the given media and indicate in aReference if 
// buffer is created.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CAmrPayloadFormatRead::CreateSourceBufferL( TMediaId aMediaId, 
        TBool &aReference )
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_AMR_PLF_READ_CREATESOURCEBUFFERL );
    #endif

    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        User::Leave( KErrNotSupported );
        }

    // the source buffers belong to AmrPayloadFormatRead not to datapath
    // aference should be set to ETrue and destried by AmrPayloadFormatRead itself.
    aReference = ETrue;
    return iFrameBufferOne; 
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::CreateSourceBufferL
// Create a source buffer for the given media, setting frame size to match
// the given sink buffer
// -----------------------------------------------------------------------------
//
CMMFBuffer* CAmrPayloadFormatRead::CreateSourceBufferL( TMediaId aMediaId,
        CMMFBuffer& /*aSinkBuffer*/, TBool &aReference )
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_AMR_PLF_READ_CREATESOURCEBUFFERL );
    #endif

    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        User::Leave( KErrNotSupported );
        }
    
    return CreateSourceBufferL( aMediaId, aReference );
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SourceDataTypeCode
// Return the source data type ( four CC code ) for the given media
// -----------------------------------------------------------------------------
//
TFourCC CAmrPayloadFormatRead::SourceDataTypeCode( TMediaId aMediaId )
    {
    if ( KUidMediaTypeAudio == aMediaId.iMediaType )
        {
        #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        	AMR_PAYLOAD_FORMAT_READ2( 
        		"CAmrPayloadFormatRead::SourceDataTypeCode: 0x%x", iFourCC.FourCC() );
        #endif
        return iFourCC;
        }
    else
        {
        return TFourCC( ); //defaults to 'NULL' fourCC
        }
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SetSourceDataTypeCode
// Set the source data type to the given four CC code for the given media
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadFormatRead::SetSourceDataTypeCode( TFourCC aSourceFourCC, 
        TMediaId aMediaId )
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        	AMR_PAYLOAD_FORMAT_READ2( 
        		"CAmrPayloadFormatRead::SetSourceDataTypeCode: 0x%x", 
        		aSourceFourCC.FourCC() );
    #endif
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %d", MCC_TRACE, MCC_AMR_PLF_READ_SETSOURCEDATATYPECODE,
            aSourceFourCC.FourCC() );
    #endif    
    
    if ( KUidMediaTypeAudio != aMediaId.iMediaType ) 
        {
        return KErrNotSupported;
        }

    iFourCC = aSourceFourCC;
    iMediaId = aMediaId;

    if ( aSourceFourCC == KMccFourCCIdAMRWB )
        {
        // Nb is set true in the constructor by default.
        iIsNb = EFalse; 
        SetSampleRate( KAmrWbSampleRate );
        }
        
    iClip->SetSourceDataTypeCode( iFourCC, iMediaId );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SourceThreadLogon
// Log in to the source thread
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadFormatRead::SourceThreadLogon( 
        MAsyncEventHandler& aEventHandler )
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_AMR_PLF_READ_SOURCETHREADLOGON );
    #endif
    
    iEventHandler = &aEventHandler;
    iClip->SourceThreadLogon( aEventHandler );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::NegotiateSourceL( MDataSink& aDataSink )
// Negotiate source settings to match data sink object.
// Re-size frame buffers if needed
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::NegotiateSourceL( MDataSink& /*aDataSink*/ )
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_AMR_PLF_READ_NEGOTIATESOURCEL );
    #endif

    iClip->NegotiateSourceL( *this );
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SourceThreadLogoff
// Log out of the source thread.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::SourceThreadLogoff( )
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_AMR_PLF_READ_SOURCETHREADLOGOFF );
    #endif

    iClip->SourceThreadLogoff( );
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::DataBufferFilledL
// Called after the data buffer is filled. Update the number of bytes read
// and the current read position for the next read operation.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::DataBufferFilledL( CMMFBuffer* aBuffer, 
                                           const TRtpRecvHeader& aRtpHeader )
    {
    AMR_PAYLOAD_FORMAT_READ2( "CAmrPayloadFormatRead::DataBufferFilledL TSTAMP: %u",
        aRtpHeader.iTimestamp )
    
    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( KUidMmfDataBuffer == aBuffer->Type(), 
        User::Leave( KErrNotSupported) );
    __ASSERT_ALWAYS( iSourceBuffer == aBuffer, User::Leave( KErrArgument ) );
    
    AMR_PAYLOAD_FORMAT_READ2( "CAmrPayloadFormatRead::DataBufferFilledL LEN:%d",
        iSourceBuffer->Data().Length() )
    
    if ( !iSourceBuffer->Data().Size() )
        {
        AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::BufferFilledL NO DATA" )
        
        // Request for a new buffer or playback is stopped
        FillSourceBufferL();
        return;
        }
        
    iBufferToRead = iSourceBuffer;
	iRecvHeader = aRtpHeader;
    iBufferToReadExists = ETrue;

    // Get a reference to the data
    TDes8& srcData = iSourceBuffer->Data();

	iPayloadDecoder->SetPayloadBuffer( srcData );
	iPayloadDecoder->SetChannelCount( iChannels );
	iPayloadDecoder->SetFrameBlockCount( iFramesPerPacket );

	// Decoder rips off redundant frames already received and increases
	// timestamp accordingly. This prevents frame placing too early at
	// jitter buffer's playout buffer.
	const TUint32 timeStampIncrement = 
	    TUint32( iCInfo.iHwFrameTime * iChannels * iSamplingRate * KTimeMultiplier );

	iNumOfFrames = iPayloadDecoder->DecodePayload( iRecvHeader.iTimestamp,
	                                               timeStampIncrement );
    
	iBufferToRead->SetFrameNumber( iRecvHeader.iTimestamp );

	// If the mode request has changed and it is a speech mode, then report
	// it to subcontroller. Otherwise it can be ignored (RFC3267 4.3.1)
	const TAmrModeRequest updateCmr = iPayloadDecoder->ModeRequest();
	if ( updateCmr != iCmr && updateCmr != EAmrModeReqNone )
	    {
        AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::BufferFilledL MODE CHANGE" )
        
        iCmr = updateCmr;
        SendAmrEventToClient( EMccInternalAmrEventCmr );
	    }
	
    AMR_PAYLOAD_FORMAT_READ2( "CAmrPayloadFormatRead::BufferFilledL MODE_REQ: %u", updateCmr )

    // Read frame data and forward it to data sink
    iCurrentFrame = 0;

    // Whenever BufferFilledL is called from RtpSourceSink
    // Set the state machine to fill sinkbuffer
    if ( iNumOfFrames )
        {
        iStateMachine->ChangeState( ESourceDataReady );
        }
    else
        {
        AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::BufferFilledL NO ACCEPTED FRAMES" )
        
        // Request for a new buffer or playback is stopped
        FillSourceBufferL();
        }
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::NumChannels
// Gets the number of channels
// -----------------------------------------------------------------------------
//
TUint CAmrPayloadFormatRead::NumChannels( )
    {
    return KMono;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SampleRate
// Returns the samplerate
// -----------------------------------------------------------------------------
//
TUint CAmrPayloadFormatRead::SampleRate( )
    {
    return iSamplingRate;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SetSampleRate
// Only supported sample rate for AMR-NB is 8000Hz
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadFormatRead::SetSampleRate ( TUint aSampleRate )
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %d", MCC_TRACE, MCC_AMR_PLF_READ_SETSAMPLERATE,
            aSampleRate );
    #endif
    
    if( (iIsNb && KAmrNbSampleRate != aSampleRate) ||
        (!iIsNb && KAmrWbSampleRate != aSampleRate) )
        {
        return KErrNotSupported;
        }
    else
        {
        iSamplingRate = aSampleRate;
        return KErrNone;
        }
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::Duration
// Return the clip duration for the given media.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CAmrPayloadFormatRead::Duration( TMediaId /*aMediaType*/ ) const
    {
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::CancelDlRequest( )
// Cancels the statemachine and resets the buffers
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::CancelDlRequest()
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::CancelDlRequest" );
    #endif

    iStateMachine->Cancel();
    iStateMachine->ChangeState( EDecodeIdle );
    
    if ( iFrameBufferOne && iFrameBufferTwo )
        {
        iFrameBufferOne->SetLastBuffer( EFalse );
        iFrameBufferTwo->SetLastBuffer( EFalse );

        iFrameBufferOne->SetStatus( EAvailable );
        iFrameBufferTwo->SetStatus( EAvailable );
        }

    iBufferToReadExists = EFalse;
    iCurrentBuffer = EBufferOne;
    iFillRequested = EFalse;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SourcePrimeL()
// 
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::SourcePrimeL()
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::SourcePrimeL" );
    #endif
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_AMR_PLF_READ_SOURCEPRIMEL );
    #endif

    iClip->SourcePrimeL();
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SourcePlayL()
// Starts the source
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::SourcePlayL()
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::SourcePlayL" );
    #endif
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_AMR_PLF_READ_SOURCEPLAYL );
    #endif
    
    #ifdef FTD_ENABLED
    iLatestSeqNum = 0;
    iTotalFrames = 0;
    iLostFrames = 0;
    #endif
    
    // Initialize payload decoder
    iPayloadDecoder->Initialize();
    
    iClip->SourcePlayL();
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SourcePauseL()
// Pauses the source
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::SourcePauseL()
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::SourcePauseL" );
    #endif
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_AMR_PLF_READ_SOURCEPAUSEL );
    #endif

    this->CancelDlRequest();
    iClip->SourcePauseL();
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SourceStopL( )
// Stops the source
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::SourceStopL()
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        AMR_PAYLOAD_FORMAT_READ(  "CAmrPayloadFormatRead::SourceStopL" );
    #endif
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_AMR_PLF_READ_SOURCESTOPL );
    #endif

    this->CancelDlRequest();
    iClip->SourceStopL();
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SinkDataTypeCode()
// Returns the datatype code for this Format Decoder
// -----------------------------------------------------------------------------
//
TFourCC CAmrPayloadFormatRead::SinkDataTypeCode( TMediaId aMediaId )
    {
    if( KUidMediaTypeAudio == aMediaId.iMediaType )
        {
        return iFourCC;
        }
    else
        {
        return TFourCC();
        }
    }
    
// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::ConfigurePayloadFormatL
// Configure payload decoding parameters.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::ConfigurePayloadFormatL( const TDesC8& aConfigParams )
    {
    AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::ConfigurePayloadFormatL (NB/WB)" );
    __ASSERT_ALWAYS( aConfigParams.Size() == sizeof( TMccCodecInfo ),
        User::Leave( KErrArgument ) );
        
    TMccCodecInfoBuffer infoBuffer;
    infoBuffer.Copy( aConfigParams );
    
    if ( !infoBuffer().iIsUpdate )
        {
        __ASSERT_ALWAYS( infoBuffer().iHwFrameTime, User::Leave( KErrArgument ) );
        
        iCInfo = infoBuffer();
        
        AMR_PAYLOAD_FORMAT_READ5 ( "CAmrPayloadFormatRead::ConfigurePayloadFormatL \
            MaxPT: %d HWFS: %d FS: %d RED: %d" , iCInfo.iMaxPtime, iCInfo.iHwFrameTime, 
            iCInfo.iFrameSize, iCInfo.iRedundancyCount );
        
        if ( !iPayloadDecoder )
            {
            if ( KAmrCodecModeBandwidthEfficient == iCInfo.iCodecMode )
                {
                iPayloadDecoder = CAmrPayloadDecoder::NewL( iIsNb );
                }
            else if ( KAmrCodecModeOctetAlign == iCInfo.iCodecMode )
                {
                iPayloadDecoder = CAmrPayloadDecoderOA::NewL( iIsNb );
                }
            else
                {
                AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::ConfigurePayloadFormatL KErrArgument" );
                User::Leave( KErrArgument );
                }
            }

        iFramesPerPacket = iCInfo.iMaxPtime / iCInfo.iHwFrameTime;
        iPayloadDecoder->SetFrameBlockCount( iFramesPerPacket );

        if ( !iFrameBufferOne )
            {
            iFrameBufferOne = CMMFDataBuffer::NewL( iCInfo.iFrameSize );
            }
        if ( !iFrameBufferTwo )
            {
            iFrameBufferTwo = CMMFDataBuffer::NewL( iCInfo.iFrameSize );
            }
            
        if ( iSourceBufOwnership )	
            {
            delete iSourceBuffer;
            }
        iSourceBuffer = NULL;
        
        TInt plSize = iCInfo.iFrameSize * iFramesPerPacket;
        
        if ( EAmrFecUsed == iCInfo.iAlgoUsed )
            {
            AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::ConfigurePayloadFormatL - Using AMR FEC" );
            
            plSize 
                = ( 0 <= iCInfo.iMaxRed )
                ? iCInfo.iFrameSize * ( iFramesPerPacket + ( iCInfo.iMaxRed / iCInfo.iHwFrameTime ) )
                : iCInfo.iFrameSize * ( iFramesPerPacket + KMaxAmrRedCount );
            }
        else if ( EGenRedUsed == iCInfo.iAlgoUsed )
            {
            AMR_PAYLOAD_FORMAT_READ2( "CAmrPayloadFormatRead::ConfigurePayloadFormatL, RED LEVEL: %d",
                iCInfo.iRedundancyCount );
            
            if ( iCInfo.iRedundancyCount )
                {
                plSize *= iCInfo.iRedundancyCount;
                }
            
            CPayloadFormatRead* redDecoder 
                = static_cast<CPayloadFormatRead*>( iClip );
            
            TMccRedPayloadReadConfig config;
            config.iRedBlockCount = iCInfo.iRedundancyCount;
            config.iMaxPayloadSize = iCInfo.iFrameSize * iFramesPerPacket;
            config.iNumOfEncodings = 1;
            config.iRedPayloadType = iCInfo.iRedundantPayload;
            config.InitPayloadTypes();
            config.iEncPayloadTypes[0] = iCInfo.iPayloadType;
            TMccRedPayloadReadPckg pckg( config );
            redDecoder->ConfigurePayloadFormatL( pckg );
            }
        else
            {
            // NOP
            }
        
        iSourceBuffer = CreateClipBufferL( plSize, iSourceBufOwnership );
        }
    else
        {
        UpdateConfigurationL( infoBuffer() );
        }            
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::UpdateConfigurationL
// Update payload decoder parameters
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::UpdateConfigurationL( const TMccCodecInfo& aCodecInfo )
    {
    AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::UpdateConfigurationL" );
    
    if ( iCInfo.iMaxPtime != aCodecInfo.iMaxPtime )
        {       
        iFramesPerPacket = aCodecInfo.iMaxPtime / iCInfo.iHwFrameTime;
        iPayloadDecoder->SetFrameBlockCount( iFramesPerPacket );
              
        if ( iSourceBufOwnership )
            {
            delete iSourceBuffer;
            }
        iSourceBuffer = NULL;
        
        TInt plSize = iCInfo.iFrameSize * iFramesPerPacket;
        
        if ( EAmrFecUsed == iCInfo.iAlgoUsed )
            {
            plSize 
                = ( 0 <= iCInfo.iMaxRed )
                ? iCInfo.iFrameSize * ( iFramesPerPacket + ( iCInfo.iMaxRed / iCInfo.iHwFrameTime ) )
                : iCInfo.iFrameSize * ( iFramesPerPacket + KMaxAmrRedCount );
            }
        else
            {
            AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::ConfigurePayloadFormatL Not using AMR FEC" )
            }           
 
        iSourceBuffer = CreateClipBufferL( plSize, iSourceBufOwnership );
        
        // Initialize payload decoder
        iPayloadDecoder->Initialize();
        
        iCInfo.iMaxPtime = aCodecInfo.iMaxPtime;
        }
    
    AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::UpdateConfigurationL OUT" )
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SendAmrEventToClient
// -----------------------------------------------------------------------------
//    
void CAmrPayloadFormatRead::SendAmrEventToClient( 
    TMccInternalEventType aEventType )
    {
    TMccAmrEventData eventData;
    TInt bitrate = ConvertModeToBitrate( iCmr );
    eventData.iModeRequestBitrate = bitrate;
    
    if ( iEventHandler )
	    {
	    TMccEvent event;
	    event.iEventData = TMccAmrEventDataPackage( eventData );
	    
	    TMccInternalEvent internalEvent( KMccAmrFormatterUid, 
	                                     aEventType,
	                                     event );
	                                     
	    iEventHandler->SendEventToClient( internalEvent );	
	    }
#ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
    else
	    {
        AMR_PAYLOAD_FORMAT_READ ( "CAmrPayloadFormatRead::SendAmrEventToClient, \
iEventHandler=NULL" )
	    }
#endif
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::CreateClipBufferL
// Creates buffer needed in data transfer with format readers clip.
// -----------------------------------------------------------------------------
//
CMMFDataBuffer* CAmrPayloadFormatRead::CreateClipBufferL( 
        TUint aSize, TBool& aIsOwnBuffer )
    {
    AMR_PAYLOAD_FORMAT_READ ( "CAmrPayloadFormatRead::CreateClipBufferL" )
    CMMFDataBuffer* buffer( NULL );
    
    if ( iClip->CanCreateSourceBuffer() )
        {
        if ( iClip->DataSourceType() == KUidMmfFormatDecode || 
             iClip->DataSourceType() == KMccMultiplexerUid )
            {
            static_cast<CMMFFormatDecode*>( iClip )->SuggestSourceBufferSize( aSize );
            }
        
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

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::ConvertModeToBitrate
// Gives corresponding bitrate for mode request depending
// on formatter mode (nb/wb)
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadFormatRead::ConvertModeToBitrate( TAmrModeRequest aModeRequest )
    {
    if ( iIsNb && aModeRequest < KNumberOfNbModes )
        {
        return KAmrNbModes[aModeRequest];
        }
    
    if ( !iIsNb && aModeRequest < KNumberOfWbModes )
        {
        return KAmrWbModes[aModeRequest];
        }
   
    return 0;
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
