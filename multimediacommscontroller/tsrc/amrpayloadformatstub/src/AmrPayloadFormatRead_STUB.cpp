/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
#include <E32base.h>
#include <mmf/common/mmffourcc.h>
#include "AmrPayloadFormatRead.h"
#include "AmrPayloadEncoder.h"
#include "AmrPayloadDecoder.h"
#include "AmrPayloadFormatUtil.h"
#include "AmrCommonUtil.h"
#include "MccRtpDataSource.h"
#include "MccRedPayloadRead.h"
#include "MccRedPayloadFormatDefs.h"
#include "mccuids.hrh"
#include "MmccCodecInformation.h"
#include "Mccdef.h"

#ifdef VOIP_TRACE_ENABLED
#include <voip_trace.h>
#endif

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS
const TUid KRedReadImplementFormat = { KImplUidRedPayloadFormatDecode };

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::CAmrPayloadFormatRead
//
// CAmrPayloadFormatRead default constructor, can NOT contain any code,
// that might leave
// Phase #1 of 2-phase constructor
// -----------------------------------------------------------------------------
//
CAmrPayloadFormatRead::CAmrPayloadFormatRead ( ) :
    iSamplingRate( 8000 ),
    iChannels( 1 ),
    iMode( 7 ),
    iPayloadDecoder( NULL ),
    iStreamEncoder( TStreamEncoder() )
    {
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::ConstructL
//
// Symbian 2nd phase constructor can leave.
// Phase #2 of 2-phase constructor
// Create an instance of the AMR-NB decoder format class
// Payload decoder ( either bandwidth efficient or octet aligned mode ) instance 
// is created in InitializeL-method.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::ConstructL ( MDataSource* /*aSource*/ )
    {
    iFourCC.Set( TFourCC( ' ','A','M','R' ) );
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::NewL
//
// Two-phased constructor.
// Static function for creating and constructing an instance of the AMR-NB format.
//
// Returns:  CAmrPayloadFormatRead* : pointer to created instance
// -----------------------------------------------------------------------------
//
CAmrPayloadFormatRead* CAmrPayloadFormatRead::NewL ( MDataSource* aSource )
    {
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
    if ( iFrameBufferOne )
        {
        delete iFrameBufferOne;
        }
    if ( iFrameBufferTwo )
        {
        delete iFrameBufferTwo;
        }
    if ( iSourceBuffer )
        {
        delete iSourceBuffer;
        }
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

    #ifdef FTD_ENABLED
    iJBufStatsQueue.Close();
    #endif
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::Streams
//
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
//
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

//    The following functions are virtual function implementation from MDataSource
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::FillBufferL
// Fill Buffer
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::FillBufferL( CMMFBuffer* /*aBuffer*/, 
                                         MDataSink* aConsumer, 
                                         TMediaId /*aMediaId*/ )
    {
    iDataPath = aConsumer;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::DoRead
// Reads data starting from the given position into the source buffer 
// -----------------------------------------------------------------------------
//
TBool CAmrPayloadFormatRead::DoRead( CMMFBuffer* /*aBuffer*/ )
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::FillSinkBuffer
//
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
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::FillSourceBufferL
//
// Send fill buffer request to RTP Data Soure
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::FillSourceBufferL( )
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        AMR_PAYLOAD_FORMAT_READ ( "CAmrPayloadFormatRead::FillSourceBufferL( )" );
    #endif
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_AMR_PLF_READ_FILLSOURCEBUFFERL );
    #endif 
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SendDataToSinkL
//
// Send full frame buffer to Data Path
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::SendDataToSinkL( )
    {
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::CreateSourceBufferL
//
// Create a source buffer for the given media and indicate in aReference if buffer
// is created.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CAmrPayloadFormatRead::CreateSourceBufferL( TMediaId /*aMediaId*/, 
                                                        TBool &aReference )
    {

    // the source buffers belong to AmrPayloadFormatRead not to datapath
    // aference should be set to ETrue and destried by AmrPayloadFormatRead itself.
    aReference = ETrue;
    
    if ( !iFrameBufferOne )
	    {
	    iFrameBufferOne = CMMFDataBuffer::NewL( 100 );
	    }
    return iFrameBufferOne;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::CreateSourceBufferL
//
// Create a source buffer for the given media, setting frame size to match
// the given sink buffer
// -----------------------------------------------------------------------------
//
CMMFBuffer* CAmrPayloadFormatRead::CreateSourceBufferL( TMediaId aMediaId,
                                                        CMMFBuffer& /*aSinkBuffer*/,
                                                        TBool &aReference )
    {

    return CreateSourceBufferL( aMediaId, aReference );
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SourceDataTypeCode
//
// Return the source data type ( four CC code ) for the given media
// -----------------------------------------------------------------------------
//
TFourCC CAmrPayloadFormatRead::SourceDataTypeCode( TMediaId aMediaId )
    {
    if ( KUidMediaTypeAudio == aMediaId.iMediaType )
        {
        return iFourCC;
        }
    else
        {
        return TFourCC( ); //defaults to 'NULL' fourCC
        }
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SetSourceDataTypeCode
//
// Set the source data type to the given four CC code for the given media
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadFormatRead::SetSourceDataTypeCode( TFourCC /*aSourceFourCC*/, 
                                                   TMediaId /*aMediaId*/ )
    {
    /*#ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
    RDebug::Print ( _L("CAmrPayloadFormatRead::SetSourceDataTypeCode") );
    #endif
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %d", Mcc_TRACE, Mcc_AMR_PLF_READ_SETSOURCEDATATYPECODE,
            aSourceFourCC.FourCC() );
    #endif    
    
    if ( KUidMediaTypeAudio != aMediaId.iMediaType ) 
        {
        return KErrNotSupported;
        }

    iFourCC = aSourceFourCC;
    iMediaId = aMediaId;

    iClip->SetSourceDataTypeCode( iFourCC, iMediaId );*/

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SourceThreadLogon
//
// Log in to the source thread
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadFormatRead::SourceThreadLogon( 
        MAsyncEventHandler& /*aEventHandler*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::NegotiateSourceL( MDataSink& aDataSink )
//
// Negotiate source settings to match data sink object.
// Re-size frame buffers if needed
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::NegotiateSourceL( MDataSink& /*aDataSink*/ )
    {

    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SourceThreadLogoff
//
// Log out of the source thread.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::SourceThreadLogoff( )
    {

    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::DataBufferFilledL
//
// Called after the data buffer is filled. Update the number of bytes read
// and the current read position for the next read operation.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::DataBufferFilledL( CMMFBuffer* /*aBuffer*/, 
                                           const TRtpRecvHeader& /*aRtpHeader*/ )
    {
    AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::BufferFilledL" );
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::NumChannels
// Gets the number of channels
// -----------------------------------------------------------------------------
//
TUint CAmrPayloadFormatRead::NumChannels( )
    {
    return iChannels;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SampleRate
//
// Get the packetization rate.
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
    
    if( KAmrNbSampleRate != aSampleRate )
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
//
// Return the clip duration for the given media.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CAmrPayloadFormatRead::Duration( TMediaId /*aMediaType*/ ) const
    {
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }


// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::CancelDlRequest( )
//
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::CancelDlRequest()
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::CancelDlRequest" );
    #endif
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
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SourcePlayL()
//
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::SourcePlayL()
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::SourcePlayL" );
    #endif
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SourcePauseL()
//
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::SourcePauseL()
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::SourcePauseL" );
    #endif
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SourceStopL( )
//
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::SourceStopL()
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_READ
        AMR_PAYLOAD_FORMAT_READ(  "CAmrPayloadFormatRead::SourceStopL" );
    #endif
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
void CAmrPayloadFormatRead::ConfigurePayloadFormatL( const TDesC8& /*aConfigParams*/ )
    {
    AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::ConfigurePayloadFormatL" );
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::UpdateConfigurationL
// Update payload decoder parameters
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatRead::UpdateConfigurationL( const TMccCodecInfo& /*aConfigParams*/ )
    {
    AMR_PAYLOAD_FORMAT_READ( "CAmrPayloadFormatRead::UpdateConfigurationL" );
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::SendAmrEventToClient
// -----------------------------------------------------------------------------
//    
void CAmrPayloadFormatRead::SendAmrEventToClient( 
    TMccInternalEventType /*aEventType*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatRead::CreateClipBufferL
// Creates buffer needed in data transfer with format readers clip.
// -----------------------------------------------------------------------------
//
CMMFDataBuffer* CAmrPayloadFormatRead::CreateClipBufferL( 
        TUint /*aSize*/, TBool& /*aIsOwnBuffer*/ )
    {
    AMR_PAYLOAD_FORMAT_READ ( "CAmrPayloadFormatRead::CreateClipBufferL" )
    return NULL;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
