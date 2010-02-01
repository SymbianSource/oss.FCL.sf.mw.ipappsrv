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
* Description:    Payload format component capable to write RTP payload
*                containing AMR audio.
*
*/




// INCLUDE FILES
#include <e32base.h>
#include <mmf/common/mmffourcc.h>
#include "amrpayloadformatwrite.h"
#include "amrpayloadencoder.h"
#include "amrpayloadencoderoa.h"
#include "amrpayloaddecoder.h"
#include "amrcommonutil.h"
#include "mccrtpdatasink.h"
#include "mccuids.hrh"
#include "mccdef.h"
#include "mccinternaldef.h"
#include "amrpayloadformatter.h"
#include "mccrtpmediaclock.h" 
#include "mccredpayloadwrite.h"

#ifdef VOIP_TRACE_ENABLED
#include <voip_trace.h>
#endif

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::CAmrPayloadFormatWrite
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAmrPayloadFormatWrite::CAmrPayloadFormatWrite( )
    : iSamplingRate( KAmrNbSampleRate ), iChannels( 1 )
    {
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::ConstructL ( MDataSink* aSink )
    {
    // Set default values
    iFramesPerPacket = 1;
    iFourCC.Set( TFourCC( ' ','A','M','R' ) );

    // Set data sink
    iIsRtpSink = ( KMccRtpSinkUid  == aSink->DataSinkType() );
    
    if ( iIsRtpSink )
        {
        CMccRtpDataSink* tmp = static_cast<CMccRtpDataSink*>( aSink );
        iRtpDataSink = static_cast<MMccRtpDataSink*>( tmp );
        }
        
    iClip = aSink;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAmrPayloadFormatWrite* CAmrPayloadFormatWrite::NewL( MDataSink* aSink )
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        RDebug::Print( _L("CAmrPayloadFormatWrite::NewL") );
    #endif
    
    __ASSERT_ALWAYS( aSink, User::Leave( KErrArgument ) );

    CAmrPayloadFormatWrite* self = new ( ELeave ) CAmrPayloadFormatWrite;
    CleanupStack::PushL( self );
    self->ConstructL( aSink );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::~CAmrPayloadFormatWrite
// Destructor.
// -----------------------------------------------------------------------------
//
CAmrPayloadFormatWrite::~CAmrPayloadFormatWrite ( )
    {
    delete iSourceBuffer;
    iRtpDataSink = NULL;
    iClip = NULL;
    delete iSinkBuffer;
    delete iStateMachine;
    delete iPayloadEncoder;
    iModes.Close();
    iRtpMediaClock = NULL;
    }

 
// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::FrameTimeInterval
//
// Return the frame time interval for the given media
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CAmrPayloadFormatWrite::FrameTimeInterval( 
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
// CAmrPayloadFormatWrite::CreateSinkBufferL
//
// Create a source buffer for the given media and indicate in aReference if buffer
// is created.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CAmrPayloadFormatWrite::CreateSinkBufferL( TMediaId aMediaId, 
                                                       TBool &aReference )
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        AMR_PAYLOAD_FORMAT_WRITE ( "CAmrPayloadFormatWrite::CreateSinkBufferL( )" );
    #endif

    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_AMR_PLF_WRITE_CREATESINKBUFFERL );
    #endif

    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        User::Leave( KErrNotSupported );
        }
    // the ownership of iSourceBuffer is in CAmrPayloadFormatWrite
    aReference = ETrue;
   	return CreateSinkBufferOfSizeL( iCInfo.iFrameSize );
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SinkDataTypeCode
//
// Return the sink data type ( four CC code ) for the given media
// -----------------------------------------------------------------------------
//
TFourCC CAmrPayloadFormatWrite::SinkDataTypeCode( TMediaId aMediaId )
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
// CAmrPayloadFormatWrite::SetSinkDataTypeCode
//
// Set the sink data type to the given four CC code for the given media
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadFormatWrite::SetSinkDataTypeCode( TFourCC aSinkFourCC, 
                                                  TMediaId aMediaId )
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %d", MCC_TRACE, MCC_AMR_PLF_WRITE_SETSINKDATATYPECODE,
            aSinkFourCC.FourCC() );
    #endif
        
    if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        return KErrNotSupported;
        }
    else 
        {
        iFourCC = aSinkFourCC;
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SinkThreadLogon
//
// Passes the logon command to the sink clip
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadFormatWrite::SinkThreadLogon( MAsyncEventHandler& aEventHandler )
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_AMR_PLF_WRITE_SINKTHREADLOGON );
    #endif
        
    iClip->SinkThreadLogon( aEventHandler );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SinkThreadLogoff
//
// Log out of the sink thread.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::SinkThreadLogoff( )
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_AMR_PLF_WRITE_SINKTHREADLOGOFF );
    #endif
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::SinkThreadLogoff " );
    #endif   
    
    iClip->SinkThreadLogoff( );
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::ProcessFramesL
//
// Packetize the AMR frames received from AMR codec and deliver the packets.
// The AMR frames are stored in "iSourceBuffer".
// return value - Current talk spurt finished, ETrue. Otherwise, EFalse.
// -----------------------------------------------------------------------------
//
TBool CAmrPayloadFormatWrite::ProcessFramesL( )
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::DeliverPacketL
//
// Prepare packet header and deliver the packet to the datasink.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::DeliverPacketL( CMMFDataBuffer& aPayload )
    {
    AMR_PAYLOAD_FORMAT_WRITE2( "CAmrPayloadFormatWrite::DeliverPacketL - TSTAMP: %u",
        static_cast<TUint32>( aPayload.TimeToPlay().Int64() ) );

    }


// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::EmptyBufferL
//
// Empty the given source buffer by formatting the AMR frames into RTP payload.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::EmptyBufferL( CMMFBuffer* /*aBuffer*/, 
                                           MDataSource* /*aSupplier*/, 
                                           TMediaId /*aMediaId*/ )
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::EmptyBufferL( )" );
    #endif
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::EmptySourceBufferL
//
// Empty the given source buffer by formatting the AMR frames into RTP payload.
// Source buffer is given in "iSourceBuffer".
// Called by the state machine.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::EmptySourceBufferL( )
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::EmptySourceBufferL" );
    #endif
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SourceBufferEmptiedL
//
// Hanlde the event that source buffer has been emptied.
// Source buffer is given in "iSourceBuffer".
// Called by the state machine.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::SourceBufferEmptiedL()
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        AMR_PAYLOAD_FORMAT_WRITE ( "CAmrPayloadFormatWrite::SourceBufferEmptiedL" );
    #endif
    }


// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::BufferEmptiedL
// Called after payload buffer is completely emptied by RtpDataSink.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
    {
    }


// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::NumChannels
//
// Get the number of channels
// -----------------------------------------------------------------------------
//
TUint CAmrPayloadFormatWrite::NumChannels( )
    {
    return iChannels;
    }


// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SampleRate
//
// Get packetization rate.
// -----------------------------------------------------------------------------
//
TUint CAmrPayloadFormatWrite::SampleRate( )
    {
    return iSamplingRate;
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SetSampleRate
// 
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadFormatWrite::SetSampleRate ( TUint aSampleRate )
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %d", MCC_TRACE, MCC_AMR_PLF_WRITE_SETSAMPLERATE,
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
// CAmrPayloadFormatWrite::Duration
//
// Return the clip duration for the given media.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CAmrPayloadFormatWrite::Duration( 
        TMediaId /*aMediaType*/ ) const
    {
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }


// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::CreateSinkBufferOfSizeL
//
// Create a sink buffer of the given size.
// -----------------------------------------------------------------------------
//
CMMFDataBuffer* CAmrPayloadFormatWrite::CreateSinkBufferOfSizeL( TUint aSize )
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        AMR_PAYLOAD_FORMAT_WRITE ( "CAmrPayloadFormatWrite::CreateSinkBufferOfSizeL( )" );
    #endif
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %d", MCC_TRACE, MCC_AMR_PLF_WRITE_CREATESINKBUFFEROFSIZEL,
            aSize );
    #endif

    if ( !iSourceBuffer )
	    {  
	    iSourceBuffer = CMMFDataBuffer::NewL( aSize );
    	iSourceBuffer->Data().FillZ( aSize );
    	iSourceBuffer->SetRequestSizeL( aSize );
	    }
    return iSourceBuffer;
    }


// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::IsBitrateChangeValid
// Checks if codec mode change request is valid.
// -----------------------------------------------------------------------------
//
TBool CAmrPayloadFormatWrite::IsBitrateChangeValid( TInt /*aBitRate*/ ) 
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::IsBitrateChangeValid" );
    #endif
    return ETrue;
    }
    
    
// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::CancelUlRequest
// Cancel UL Request
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::CancelUlRequest( )
    {
    #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
        AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::CancelUlRequest" );
    #endif
    }

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SinkPrimeL()
// Primes sink
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::SinkPrimeL()
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_AMR_PLF_WRITE_SINKPRIMEL );
    #endif
    
    iClip->SinkPrimeL();
    };

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SinkPlayL()
// Plays sink
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::SinkPlayL()
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %d", MCC_TRACE, MCC_AMR_PLF_WRITE_SINKPLAYL, iCInfo.iRedundancyCount );
    #endif
    iClip->SinkPlayL();
    };

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SinkPauseL()
// Pauses sink
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::SinkPauseL()
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_AMR_PLF_WRITE_SINKPAUSEL );
    #endif
    
    this->CancelUlRequest();
    iClip->SinkPauseL();
    };

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::SinkStopL()
// Stops sink
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::SinkStopL()
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_AMR_PLF_WRITE_SINKSTOPL );
    #endif

    this->CancelUlRequest();
    iClip->SinkStopL();
    };

// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::ConfigurePayloadFormatL
// Configure payload encoding parameters.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::ConfigurePayloadFormatL( 
        const TDesC8& aConfigParams, 
        CMccRtpMediaClock& /*aClock*/  )
    {
    if ( aConfigParams.Size() == sizeof( TMccCodecInfo ) )
        {
        #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
            AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::ConfigurePayloadFormatL IN" );
        #endif
        
        TMccCodecInfoBuffer infoBuffer;
        infoBuffer.Copy( aConfigParams );
        iCInfo = infoBuffer();
        }
    else
        {
        #ifdef TRACE_AMR_PAYLOAD_FORMAT_WRITE
            AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::ConfigurePayloadFormatL KErrArgument 2" );
        #endif
        
        User::Leave( KErrArgument );
        }
    }
    
// -----------------------------------------------------------------------------
// CAmrPayloadFormatWrite::UpdateConfigurationL
// Update payload encoding parameters
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatWrite::UpdateConfigurationL( const TMccCodecInfo& aCodecInfo )
    {
    AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::UpdateConfigurationL" )

    
    iCInfo.iNeighbor = aCodecInfo.iNeighbor;                            
    iCInfo.iModeChangePeriod = aCodecInfo.iModeChangePeriod;          
    iCInfo.iRedundancyCount = aCodecInfo.iRedundancyCount;
    iCInfo.iPtime = aCodecInfo.iPtime;
    
    TUint tempMask(0x0001);
    
    // Reset old iModes array if new modes exist
    if ( iCInfo.iBitrateMask != 0x0000 )
        {
        iModes.Reset();
        }
    
    for ( TInt ii = KNumValue0; ii < KNumberOfNbModes; ii++ )
        {
        if( iCInfo.iBitrateMask & tempMask )
            {
            iModes.AppendL( KAmrNbModes[ii] );
            }
        tempMask*=2;
        } 
    
    iFramesPerPacket = iCInfo.iPtime / iCInfo.iHwFrameTime;
    iPayloadEncoder->SetFrameBlockCount( iFramesPerPacket );
    
    if ( EAmrFecUsed == iCInfo.iAlgoUsed )
        {                           
        iPayloadEncoder->Initialize();
        User::LeaveIfError( iPayloadEncoder
            ->SetRedFrameBlockCount( iCInfo.iRedundancyCount ) );
        }
    else
        {
        AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::ConfigurePayloadFormatL - Not using AMR FEC" );
        }
    
    // Allocate buffer for data transfer between
    // FormatWrite - MDataSink AND FormatWrite - redundancy payload encoder
    delete iSinkBuffer;
    iSinkBuffer = NULL;
    iSinkBuffer = CMMFDataBuffer::NewL( iCInfo.iFrameSize * iFramesPerPacket
        * ( 1 + iCInfo.iRedundancyCount ) + KNumValue2 );
    
    iPayloadEncoder->Initialize();
    TDes8& dataDes = static_cast<CMMFDataBuffer*>( iSinkBuffer )->Data( );
    dataDes.SetLength( 0 );   // for first packet length
    iPayloadEncoder->SetPayloadBuffer( dataDes );
    iPayloadEncoder->SetChannelCount( iChannels );
    
    // Reset flag
    iNeedToFlush = EFalse;
    
    AMR_PAYLOAD_FORMAT_WRITE( "CAmrPayloadFormatWrite::UpdateConfigurationL OUT" )
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
