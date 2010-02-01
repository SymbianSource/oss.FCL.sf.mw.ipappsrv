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
#include "mcch263payloadformatwrite.h"
#include "mccrtpdatasink.h"
#include "mccinternaldef.h"
#include "mcch263oldpayloadencoder.h"
#include "mcch263newpayloadencoder.h"
#include "mmcccodech263.h"
#include "formatstatemachine.h"
#include "mcch263formatlogs.h"

#include "mmccnetworksettings.h"

#ifdef VOIP_TRACE_ENABLED
#include <voip_trace.h>
#endif

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// Camera may give very big H.263 frame if there's big change in scene
const TInt KH263DefaultPayloadSize = 12000; 

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::CMccH263PayloadFormatWrite
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccH263PayloadFormatWrite::CMccH263PayloadFormatWrite()
    {
    }

// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccH263PayloadFormatWrite::ConstructL( MDataSink* aSink )
    {        
    __H263FORMAT_CONTROLL( "CMccH263PayloadFormatWrite::ConstructL" )

    iFourCC = TFourCC( KMccFourCCIdH263 );
    
    iIsRtpSink = ( KMccRtpSinkUid  == aSink->DataSinkType() );
    iClip = aSink;
        
    iStateMachine = CFormatEncodeStateMachine::NewL( this );
    iStateMachine->ChangeState( EEncodeIdle );
    }

// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMccH263PayloadFormatWrite* CMccH263PayloadFormatWrite::NewL( MDataSink* aSink )
    {
    __ASSERT_ALWAYS( aSink, User::Leave( KErrArgument ) );

    CMccH263PayloadFormatWrite* self = new( ELeave ) CMccH263PayloadFormatWrite;
    
    CleanupStack::PushL( self );
    self->ConstructL( aSink );
    CleanupStack::Pop( self );

    return self;
    }

CMccH263PayloadFormatWrite::~CMccH263PayloadFormatWrite()
    {
    iBuffers.ResetAndDestroy();
    delete iSourceBuffer;    
    delete iStateMachine;
	delete iPayloadEncoder; 
	
    iClip = NULL;
    }


// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::SinkThreadLogon
// Passes the logon command to the sink clip.
// -----------------------------------------------------------------------------
//
TInt CMccH263PayloadFormatWrite::SinkThreadLogon( 
    MAsyncEventHandler& aEventHandler )
    {
    __H263FORMAT_CONTROLL( "CMccH263PayloadFormatWrite::SinkThreadLogon" )
    
    iStateMachine->ChangeState( EEncodeIdle );
    return iClip->SinkThreadLogon( aEventHandler );
    }

// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::SinkThreadLogoff
// Passes the logoff command to the sink clip.
// -----------------------------------------------------------------------------
//
void CMccH263PayloadFormatWrite::SinkThreadLogoff()
    {
    __H263FORMAT_CONTROLL( "CMccH263PayloadFormatWrite::SinkThreadLogoff" )

    if( iClip )
	    {
	   	iClip->SinkThreadLogoff();
	    }
    else
	    {
	    __H263FORMAT_CONTROLL( "CMccH263PayloadFormatWrite::SinkThreadLogoff\
	                            iClip is NULL, Error!..." )
	    }
    }

// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::CanCreateSinkBuffer
// EmptyBufferL() is not used.
// -----------------------------------------------------------------------------
//
TBool CMccH263PayloadFormatWrite::CanCreateSinkBuffer()
    {
    __H263FORMAT_CONTROLL( "CMccH263PayloadFormatWrite::CanCreateSinkBuffer" )
    return ETrue;
    }
	    
// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::CreateSinkBufferL
// Create a sink buffer.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccH263PayloadFormatWrite::CreateSinkBufferL( 
	TMediaId /*aMediaId*/,
    TBool&     aReference )
    {
    __H263FORMAT_CONTROLL( "CMccH263PayloadFormatWrite::CreateSinkBufferL" )

    if ( !iSourceBuffer )
	    {  
	    iSourceBuffer = CMMFDataBuffer::NewL( KH263DefaultPayloadSize );
    	iSourceBuffer->Data().FillZ( KH263DefaultPayloadSize );
    	iSourceBuffer->SetRequestSizeL( KH263DefaultPayloadSize );
	    }
	// the ownership of iSourceBuffer is in CMccH263PayloadFormatWrite
	aReference = ETrue;  
    return iSourceBuffer;
    }
    

// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::SetSinkDataTypeCode
// Set the sink data type to the given FourCC code for the given media.
// -----------------------------------------------------------------------------
//
TInt CMccH263PayloadFormatWrite::SetSinkDataTypeCode( 
	TFourCC  aSinkFourCC, 
    TMediaId aMediaId )
    {
    __H263FORMAT_CONTROLL( "CMccH263PayloadFormatWrite::SetSinkDataTypeCode" )

    if ( KUidMediaTypeVideo != aMediaId.iMediaType || 
         iFourCC != aSinkFourCC )
        {
        return KErrNotSupported;
        }
    else
	    {
	    iMediaId = aMediaId;    
        return KErrNone;	
	    }
    }

// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::SinkDataTypeCode
// Return the sink data type (four CC code) for the given media.
// -----------------------------------------------------------------------------
//
TFourCC CMccH263PayloadFormatWrite::SinkDataTypeCode( TMediaId aMediaId )
    {
    __H263FORMAT_CONTROLL( "CMccH263PayloadFormatWrite::SinkDataTypeCode" )
   
    if ( KUidMediaTypeVideo == aMediaId.iMediaType )
        {
        return iFourCC;
        }
    else
        {
        return TFourCC(); //defaults to 'NULL' fourCC
        }
    }


// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::FrameTimeInterval
// Gets audio frame size in milliseconds.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds 
    CMccH263PayloadFormatWrite::FrameTimeInterval( 
    TMediaId aMediaType ) const
    {
    if( KUidMediaTypeVideo != aMediaType.iMediaType )
	    {
	    __H263FORMAT_CONTROLL( "CMccH263PayloadFormatWrite::FrameTimeInterval \
aMediaType is not KUidMediaTypeVideo, Error!!!" )	
	    }
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }

// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::Duration
// Gets the duration of the sink clip for the specified media ID.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds 
    CMccH263PayloadFormatWrite::Duration( TMediaId aMediaType ) const
    {
    if( KUidMediaTypeVideo != aMediaType.iMediaType )
	    {
	    __H263FORMAT_CONTROLL( "CMccH263PayloadFormatWrite::Duration \
aMediaType is not KUidMediaTypeVideo, Error!!!" )
	    }
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }

// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::BufferEmptiedL
// Called by RTP data sink. Indicates that DTMF data is sent to the network.
// -----------------------------------------------------------------------------
//
void CMccH263PayloadFormatWrite::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
    {
    __H263FORMAT_MEDIA( "CMccH263PayloadFormatWrite::BufferEmptiedL" )
    }
    
// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::SinkPrimeL
// Passes prime transition to the RTP data sink.
// -----------------------------------------------------------------------------
//
void CMccH263PayloadFormatWrite::SinkPrimeL()
    {
    __H263FORMAT_CONTROLL( "CMccH263PayloadFormatWrite::SinkPrimeL" )
    __ASSERT_ALWAYS( iClip, User::Leave( KErrArgument ) );

    iClip->SinkPrimeL();
    }

// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::SinkPlayL
// Passes play transition to the RTP data sink and resets state variables.
// -----------------------------------------------------------------------------
//
void CMccH263PayloadFormatWrite::SinkPlayL()
    {
    __H263FORMAT_CONTROLL( "CMccH263PayloadFormatWrite::SinkPlayL" )    
    __ASSERT_ALWAYS( iClip, User::Leave( KErrArgument ) );
    
    iClip->SinkPlayL();
    }
    
// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::SinkPauseL
// Passes pause transition to the RTP data sink.
// -----------------------------------------------------------------------------
//
void CMccH263PayloadFormatWrite::SinkPauseL()
    {
    __H263FORMAT_CONTROLL( "CMccH263PayloadFormatWrite::SinkPauseL" )
    __ASSERT_ALWAYS( iClip, User::Leave( KErrArgument ) );
    
    iClip->SinkPauseL();
    }

// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::SinkStopL
// Passes stop transition to the RTP data sink.
// -----------------------------------------------------------------------------
//  
void CMccH263PayloadFormatWrite::SinkStopL()
    {
    __H263FORMAT_CONTROLL( "CMccH263PayloadFormatWrite::SinkStopL" )
    __ASSERT_ALWAYS( iClip, User::Leave( KErrArgument ) );

    iClip->SinkStopL();
    }

// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::EmptyBufferL
// Not used because DTMF payload formatter generates DTMF independently.
// -----------------------------------------------------------------------------
//
void CMccH263PayloadFormatWrite::EmptyBufferL( 
	CMMFBuffer*  aBuffer, 
    MDataSource* aSupplier, 
    TMediaId   /*aMediaId*/
                                          )
    {
    __H263FORMAT_MEDIA( "CMccH263PayloadFormatWrite::EmptyBufferL" )
    __ASSERT_ALWAYS ( aBuffer, User::Leave(KErrArgument) );
    // reference of iSourceBuffer is passed to ULdatapath 
    // by calling CreateSinkBuffer
    __ASSERT_ALWAYS ( aBuffer == iSourceBuffer, User::Leave(KErrArgument) );
    __ASSERT_ALWAYS ( aSupplier, User::Leave(KErrArgument) );
    __ASSERT_ALWAYS ( iPayloadEncoder, User::Leave(KErrArgument) );
    __ASSERT_ALWAYS ( iStateMachine, User::Leave(KErrArgument) );

    // Save source buffer parameters and change the state.
    iDataSource = aSupplier;
    
	iBuffers.ResetAndDestroy();
	
	CleanupStack::PushL( TCleanupItem( PayloadEncodeCleanup, this ) );
    iPayloadEncoder->EncodeL(aBuffer, iBuffers);
    CleanupStack::Pop( ); // TCleanupItem
    
    __H263FORMAT_MEDIA( "CMccH263PayloadFormatWrite::EmptyBufferL, \
array count ..." )
    __H263FORMAT_MEDIA_INT1( "CMccH263PayloadFormatWrite::EmptyBufferL, \
array count:", iBuffers.Count())
            
    // empty the encoded data to RtpSink
    iStateMachine->ChangeState( EEmptySourceBuffer );     
    }


// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::ConfigurePayloadFormatL
// Configure payload encoding parameters.
// -----------------------------------------------------------------------------
//
void CMccH263PayloadFormatWrite::ConfigurePayloadFormatL(
    const TDesC8& aConfigParams, 
    CMccRtpMediaClock& /*aClock*/  )

    {   
    __ASSERT_ALWAYS( aConfigParams.Size() == sizeof( TMccCodecInfo ),
    				 User::Leave( KErrArgument ) );

    TMccCodecInfoBuffer infoBuffer;
    infoBuffer.Copy( aConfigParams );
    iCInfo = infoBuffer();
    
    if ( !iPayloadEncoder )
	    {
	    // Don't use old mode encoder for 1998 or 2000 modes, codec mode
	    // old means 1998 mode in configuration parameters
	    if( iCInfo.iCodecMode == KH263FormatModeOld ||
	        iCInfo.iCodecMode == KH263FormatModeNew )
	    	{
			iPayloadEncoder = CMccH263NewPayloadEncoder::NewL();
	    	}
	    else
	    	{
	    	User::Leave( KErrNotSupported );
	    	}
	    }	    
    iPayloadEncoder->SetPayloadType( iCInfo.iPayloadType );        
    }
    
    
// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::EmptySourceBufferL
// -----------------------------------------------------------------------------
//
void CMccH263PayloadFormatWrite::EmptySourceBufferL()
	{
    __H263FORMAT_MEDIA( "CMccH263PayloadFormatWrite::EmptySourceBufferL \
from state machine" )	
    __ASSERT_ALWAYS ( iStateMachine, User::Leave(KErrArgument) );
    __ASSERT_ALWAYS ( iClip, User::Leave(KErrArgument) );
    
    // tell the source to give more data
    iStateMachine->ChangeState( ESourceBufferEmptied );  
    
	while ( iBuffers.Count() )
    	{
	    __H263FORMAT_MEDIA( "CMccH263PayloadFormatWrite::EmptySourceBufferL \
sending" )
    	CBufferContainer* container = iBuffers[0];
    	if ( iIsRtpSink )
    	    {
        	CMccRtpDataSink* rtpSink = static_cast<CMccRtpDataSink*>( iClip );
    	    rtpSink->EmptyBufferL( container->iBuffer, this, iMediaId, container->iSendHeader );
    	    }
    	else
    	    {
    	    __H263FORMAT_MEDIA( "CMccH263PayloadFormatWrite::EmptySourceBufferL copying values" )
    	    container->iBuffer->SetTimeToPlay( container->iSendHeader.iTimestamp );
		    container->iBuffer->SetLastBuffer( container->iSendHeader.iMarker );
    	    iClip->EmptyBufferL( container->iBuffer, this, iMediaId );
    	    }
    	iBuffers.Remove(0);
    	delete container->iBuffer;
    	container->iBuffer = NULL;
    	delete container;
    	container = NULL;
    	}
    iBuffers.ResetAndDestroy();
    	
    __H263FORMAT_MEDIA( "CMccH263PayloadFormatWrite::EmptySourceBufferL \
all data sent" )
   	}

// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::SourceBufferEmptiedL
// -----------------------------------------------------------------------------
//
void CMccH263PayloadFormatWrite::SourceBufferEmptiedL()
	{
    __H263FORMAT_MEDIA( "CMccH263PayloadFormatWrite::EmptySourceBufferL from state machine" )
   
    __ASSERT_ALWAYS( iDataSource, User::Leave( KErrArgument ) );

	iDataSource->BufferEmptiedL( iSourceBuffer );   
	}

// -----------------------------------------------------------------------------
// CMccH263PayloadFormatWrite::PayloadEncodeCleanup
// -----------------------------------------------------------------------------
// 
void CMccH263PayloadFormatWrite::PayloadEncodeCleanup( TAny* aAny )
	{
	// Error occured at payload encoding, cleanup and keep on asking more data
	// to be encoded.
	CMccH263PayloadFormatWrite* write = 
		static_cast<CMccH263PayloadFormatWrite*>( aAny );
	write->iBuffers.ResetAndDestroy();
	
	write->iStateMachine->ChangeState( ESourceBufferEmptied ); 
	}

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
