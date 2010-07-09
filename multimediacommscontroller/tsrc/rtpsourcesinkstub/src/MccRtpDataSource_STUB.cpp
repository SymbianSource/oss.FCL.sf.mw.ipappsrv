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
* Description:    RTP Datasource
*
*/




// INCLUDE FILES
#include <in_sock.h>
#include <mmf/common/mmfcontroller.h>
#include <mmf/server/mmfvideoframebuffer.h>
#include <mmf/server/mmfdatabuffer.h>
#include "rtpapi.h"
#include "mccrtpdatasource.h"
#include "mccinternalevents.h"
#include "formatstatemachine.h"
#include "mccrtpdefs.h"
#include "mmccinterfacedef.h"
#include "mccinternaldef.h"

// MACROS
#define MCC_RTPSOURCE_ENDPOINT_ID reinterpret_cast<TUint32>( static_cast<MDataSource*>( this ) )


// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccRtpDataSource::CMccRtpDataSource
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccRtpDataSource::CMccRtpDataSource() : 
    CMccDataSource( KMccRtpSourceUid ),
    MMccRtpInterface(),
    iStandByTimerValue( KRtpStandByTimer ),
    iRtpStreamId( KNullId )
    {
    iCurRecvPayloadType = KMccPTNotDefined;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::ConstructSourceL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::ConstructSourceL( const TDesC8& /*aInitData*/ )
    {
    TRACE_RTP_SOURCE_PRINT ( "CMccRtpDataSource::ConstructSourceL" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::NewSourceL
// Static constructor.
// -----------------------------------------------------------------------------
//
MDataSource* CMccRtpDataSource::NewSourceL( TUid /*aImplementationUid*/, 
                                            const TDesC8& /*aInitData*/ )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::NewSourceL" )

    CMccRtpDataSource* self = new ( ELeave ) CMccRtpDataSource();
    return static_cast<MDataSource*>( self );
    }


// -----------------------------------------------------------------------------
// CMccRtpDataSource::~CMccRtpDataSource()
// Destructor.
// -----------------------------------------------------------------------------
//
CMccRtpDataSource::~CMccRtpDataSource()
    {
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::~CMccRtpDataSource 0x%x", this )
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::~CMccRtpDataSource RTCP DELETE" )
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::~CMccRtpDataSource BUFFER DELETE" )
    if ( iReference )
        {
        delete iBuffer;
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::FillBufferL
// FillBufferL works synchronously.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::FillBufferL( CMMFBuffer* aBuffer, 
                                     MDataSink*  aConsumer, 
                                     TMediaId    /*aMediaId*/ )
    {    
    TRACE_RTP_SOURCE_PRINT ( "CMccRtpDataSource::FillBufferL" )
    
    __ASSERT_ALWAYS ( aConsumer, User::Leave( KErrArgument ) );
    
    if ( aBuffer )
        {
        __ASSERT_ALWAYS ( KUidMmfDataBuffer == aBuffer->Type( ), 
            User::Leave( KErrNotSupported ) );
        }
        
    iBufferToFill = static_cast<CMMFDataBuffer*>( aBuffer );
    iFillBufferRequester = aConsumer;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::CanCreateSourceBuffer
// NOT SUPPORTED. MDataSource pure virtual function must to be implemented.
// -----------------------------------------------------------------------------
//
TBool CMccRtpDataSource::CanCreateSourceBuffer()
    {
    return iCanCreateSourceBuffer;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::CreateSourceBufferL
// NOT SUPPORTED. MDataSource pure virtual function must to be implemented.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccRtpDataSource::CreateSourceBufferL( TMediaId /*aMediaId*/,
                                                    TBool& aReference )
    {
    TRACE_RTP_SOURCE_PRINT ( "CMccRtpDataSource::CreateSourceBufferL KErrNotSupported" )
    
    if ( !iCanCreateSourceBuffer )
        {
        User::Leave( KErrNotSupported );
        }
    aReference = iReference;
    if ( iReference )
        {
        delete iBuffer;
        }
    iBuffer = NULL;
    if ( iWrongBufferType )
        {
        iBuffer = CMMFYUVBuffer::NewL();
        }
    else
        {
        iBuffer = CMMFDataBuffer::NewL( 100 );
        }
    return iBuffer;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::RtpPacketReceived
// RTP stack callback function for received RTP packet.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::RtpPacketReceived( TRtpId /*aStreamId*/, 
                                           const TRtpRecvHeader& /*aHeaderInfo*/, 
                                           const TDesC8& /*aPayloadData*/ )
	{

	}

// -----------------------------------------------------------------------------
// CMccRtpDataSource::RtpPacketReceivedL
// RTP stack callback function for received RTP packet.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::RtpPacketReceivedL( TRtpId /*aStreamId*/, 
                                           const TRtpRecvHeader& /*aHeaderInfo*/, 
                                           const TDesC8& /*aPayloadData*/ )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::RtpPacketReceived" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SetSourceDataTypeCode
// Sets the sources datatype code ( Codec )
// -----------------------------------------------------------------------------
//
TInt CMccRtpDataSource::SetSourceDataTypeCode( TFourCC /*aCodec*/, TMediaId /*aMedia*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SourceDataTypeCode()
// Sets the datatype code ( codec )
// -----------------------------------------------------------------------------
//
TFourCC CMccRtpDataSource::SourceDataTypeCode( TMediaId /*aMediaId*/ )
    {
    return TFourCC();
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::BufferEmptiedL
// CMccRtpDataSource supports only passive mode of operation. 
// Thus, Datapath->EmptyBufferL isn't called.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::BufferEmptiedL KErrNotSupported" )
    
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SourceThreadLogon
//
// Method to 'logon' the data source to the same thread that source will be 
// consuming data in. Thread specific initialisation is done here.
// -----------------------------------------------------------------------------
//
TInt CMccRtpDataSource::SourceThreadLogon( MAsyncEventHandler& /*aEventHandler*/ )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SourceThreadLogon" )
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SourceThreadLogoff
//
// Method to 'logoff' the data source from the same thread that source consumes 
// data in. Thread specific releasing of resources is done here.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::SourceThreadLogoff()
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SourceThreadLogoff" )
    }

// ---------------------------------------------------------------------------
// CMccRtpDataSource::SourcePrimeL
// Source must be primed before playing.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::SourcePrimeL()
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SourcePrimeL" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SourcePlayL
// Start receiving RTP packets.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::SourcePlayL()
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SourcePlayL" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SourcePauseL
// Pause RTP packet receiving.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::SourcePauseL()
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SourcePauseL" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SourceStopL
// Stop RTP packet receiving.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::SourceStopL()
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SourceStopL" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::PlayoutRtpPacketL
// Pass filled buffer to the data sink of RTP data source.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::PlayoutRtpPacketL( const TRtpRecvHeader& /*aHeaderInfo*/, 
        const TDesC8& /*aPayloadData*/ )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::PlayoutRtpPacketL" )
    __ASSERT_ALWAYS( iBufferToFill, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iFillBufferRequester, User::Leave( KErrNotReady ) );
    }


// -----------------------------------------------------------------------------
// CMccRtpDataSource::DoCreateStreamL
// Creates a receive stream.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::DoCreateStreamL()
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::DoCreateStreamL IN !!!!" )
    
    iRtpStreamId = 2;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::NegotiateSourceL
// Derived from MDataSource
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::NegotiateSourceL( MDataSink& aDataSink )
    {
    iFillBufferRequester = &aDataSink;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SendMediaSignallingL
// Derived from CRtpInterface
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::SendMediaSignallingL( const TMccEvent& /*aEvent*/ )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SendMediaSignallingL" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::StartInactivityTimer
// Starts inactivity timer for a stream 
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::StartInactivityTimerL( TUint32 /*aTimeoutTime*/ )
    {
    TRACE_RTP_SOURCE_PRINT ( "CMccRtpDataSource::StartInactivityTimerL" )
    }



// -----------------------------------------------------------------------------
// CMccRtpDataSource::StopInactivityTimer
// Stops inactivity timer for a stream
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::StopInactivityTimerL( )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::StopInactivityTimerL" )
    }
    
    
// -----------------------------------------------------------------------------
// CMccRtpDataSource::StandBy
// Stops inactivity timer for a stream
// -----------------------------------------------------------------------------
//
TInt CMccRtpDataSource::StandBy( TMccStandbyActionType /*aActionType*/, TUint /*aPayloadType*/ )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::StandBy" )
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::StartRtpStandByTimerL
// Starts rtp StandBy timer 
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::StartRtpStandByTimerL( TMccRtpUser* /*aUser*/ )
	{
	TRACE_RTP_SOURCE_PRINT ( "CMccRtpDataSource::StartRtpStandByTimerL" )
	}
	
// -----------------------------------------------------------------------------
// CMccRtpDataSource::StopRtpStandByTimers
// Stop rtp StandBy timer 
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::StopRtpStandByTimers()
	{
	TRACE_RTP_SOURCE_PRINT ( "CMccRtpDataSource::StopRtpStandByTimers" )
	}

// -----------------------------------------------------------------------------
// CMccRtpDataSource::HandleStandByL
// -----------------------------------------------------------------------------
//	
void CMccRtpDataSource::HandleStandByL( TMccRtpUser* /*aUser*/ )    
	{
	}

// -----------------------------------------------------------------------------
// CMccRtpDataSource::ResetStandBy
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::ResetStandBy()
    {
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::DoStandByDecision
// -----------------------------------------------------------------------------
//   	
void CMccRtpDataSource::DoStandByDecision( TMccRtpUser* /*aUser*/ )
    {
    }
 
// -----------------------------------------------------------------------------
// CMccRtpDataSource::FindUserEntryByPayloadType
// -----------------------------------------------------------------------------
//   
TMccRtpUser* CMccRtpDataSource::FindUserEntryByPayloadType( TUint /*aPayloadType*/ )
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::FindUserEntryByTimerId
// -----------------------------------------------------------------------------
//    	
TMccRtpUser* CMccRtpDataSource::FindUserEntryByTimerId( TMccTimerId /*aTimerId*/ )
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::FindUserEntryIndex
// -----------------------------------------------------------------------------
//
TInt CMccRtpDataSource::FindUserEntryIndex( TMccRtpUser* /*aUser*/ )
    {
    return KErrNotFound;
    }
 
// -----------------------------------------------------------------------------
// CMccRtpDataSource::RtpUserMatch
// -----------------------------------------------------------------------------
//  	
TBool CMccRtpDataSource::RtpUserMatch( const TMccRtpUser& /*aUser1*/, 
    	            const TMccRtpUser& /*aUser2*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::RtpStreamId
// -----------------------------------------------------------------------------
//    
TRtpId CMccRtpDataSource::RtpStreamId()
    {
    return iRtpStreamId;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::DoCreateSrtpStreamL
// -----------------------------------------------------------------------------
// 
void CMccRtpDataSource::DoCreateSrtpStreamL()
    {
    }
    
// -----------------------------------------------------------------------------
// CMccRtpDataSource::RegisterPayloadTypesL
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::RegisterPayloadTypesL( const RArray<TUint>& /*aPayloadTypes*/ )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::RegisterPayloadTypesL" );
    }
            
// -----------------------------------------------------------------------------
// CMccRtpDataSource::UnRegisterPayloadTypes
// Unregisters payload types to accept.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::UnRegisterPayloadTypes( const RArray<TUint>& /*aPayloadTypes*/ )
    {

    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::ValidatePacketL
// Validates the received RTP packet and it's header
// -----------------------------------------------------------------------------
//
TMccRtpUser* CMccRtpDataSource::ValidatePacketL( const TRtpId /*aStreamId*/, 
        const TRtpRecvHeader& /*aHeader*/, const TDesC8& /*aData*/ )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::ValidatePacket" )
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::AddUserL
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::AddUserL( TUint32 /*aStreamId*/ )
    {

    }
    
// -----------------------------------------------------------------------------
// CMccRtpDataSource::SendStreamEventToClient()
// -----------------------------------------------------------------------------
//	
void CMccRtpDataSource::SendStreamEventToClient( 
    TMccEventType /*aEventType*/,
    TInt /*aError*/,
    TUint32 /*aTargetPayloadType*/ )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SendStreamEventToClient" )

    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SendJitterEvent()
// -----------------------------------------------------------------------------
//	
void CMccRtpDataSource::SendJitterEvent( TMccRtpEventDataExtended /*aEvent*/, TInt /*aError*/ )
    {

    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SourceCustomCommand()
// 
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::SourceCustomCommand( TMMFMessage& /*aMessage*/ )
    {

    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SourceCustomCommandL()
// Worker function for TRAP'ping possible leaves that custom commands may have
// -----------------------------------------------------------------------------
//    
void CMccRtpDataSource::SourceCustomCommandL( TMMFMessage& /*aMessage*/ )
    {

    }

// ---------------------------------------------------------------------------
// FROM SRTP API
// This function is called by SRTP Stream initiated with 
// MSRTPReKeyingObserver when a master key is stale and needs 
// to be refreshed.  
// ---------------------------------------------------------------------------
//
void CMccRtpDataSource::SRTPMasterKeyStaleEvent( const CSRTPStream& /*aStream*/ )
    {
    TRACE_RTP_INTERFACE_PRINT( "CMccRtpStream::SRTPMasterKeyStaleEvent" )
    }

// ---------------------------------------------------------------------------
// FROM SRTP API
// This function is called by SRTP Stream initiated with 
// CSRTPSession  when a master key is stale and
// needs to be refreshed.  
// ---------------------------------------------------------------------------
void CMccRtpDataSource::SRTPMasterKeyStaleEvent(const CSRTPSession& /*aSession*/ )
    {
    TRACE_RTP_INTERFACE_PRINT( "CMccRtpStream::SRTPMasterKeyStaleEvent" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::TimerExpiredL
// From MMccExpirationHandler
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::TimerExpiredL( TMccTimerId /*aTimerId*/, TAny* /*aTimerParam*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::RateAdaptationRequest
// -----------------------------------------------------------------------------
//     
TInt CMccRtpDataSource::RateAdaptationRequest( const TMccEvent& aInputData, TMccEvent& aOutputData )
    {
    TInt error = 0;
    #ifdef RATECONTROL    
    TMccRateAdaptationEventDataPackage inputdatapkg;
	inputdatapkg.Copy(aInputData.iEventData);
	TMccRateAdaptationEventData inputdata = inputdatapkg();
    TMccRateAdaptationEventData outputdata =TMccRateAdaptationEventData();
    if ( ETrue)
    	{
    	TInt bitrate = 127000;
    	TReal frameRate = 15;
		outputdata.iBitrateOriginal = bitrate;			
		 __TRACE_MCC_QOS_CONTROLLER3("CMccRtpDataSource_STUB::RateAdaptationRequest originalBitrate=" , bitrate);
		TInt mbitrate = TInt( inputdata.iRateAdaptationAdvice * TReal( bitrate ) );		
		if ( mbitrate > 128000 )
			{
			mbitrate = 128000;
			}
		outputdata.iBitrateModified = mbitrate;
		__TRACE_MCC_QOS_CONTROLLER3("CMccRtpDataSource_STUB::RateAdaptationRequest modifiedBitrate=" , mbitrate);
		#ifdef FRAMERATECONTROL 	
		if ( bitrate > KIPULFrameRate10Limit )
			{
		    __TRACE_MCC_QOS_CONTROLLER3("CMccRtpDataSource_STUB::RateAdaptationRequest frameRate=" , frameRate);
			frameRate = iFrameRate;
			}
		else if (bitrate < KIPULFrameRate5Limit )
			{
			__TRACE_MCC_QOS_CONTROLLER3("CMccRtpDataSource_STUB::RateAdaptationRequest frameRate=" , frameRate);
			frameRate = Min( 5.0, iFrameRate );
			}
		else 
			{
			__TRACE_MCC_QOS_CONTROLLER3("CMccRtpDataSource_STUB::RateAdaptationRequest frameRate=" , frameRate);
			frameRate = Min( 10.0, iFrameRate );
			}
	
		#else //FRAMERATECONTROL 
		error = 0;
		#endif //FRAMERATECONTROL 
			
		TMccRateAdaptationEventDataPackage temp( outputdata );
		aOutputData.iEventData.Copy( temp );
		__TRACE_MCC_QOS_CONTROLLER1("CMccRtpDataSource_STUB::RateAdaptationRequest end");
		return KErrNone;		
    	}
#else //RATECONTROL
	error = aInputData.iErrorCode; //remove warning
    error = aOutputData.iErrorCode;
#endif //RATECONTROL
    return KErrNotSupported;
    }
        
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
