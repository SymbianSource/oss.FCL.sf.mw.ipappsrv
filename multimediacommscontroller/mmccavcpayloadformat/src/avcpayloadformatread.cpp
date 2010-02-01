/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    AVC Payload Format Read .. DePayloadizes incoming RTP packets
*
*/




// INCLUDE FILES
#include <e32base.h>
#include <mmf/common/mmffourcc.h>
#include <Sysutil.h>
#include "MccRtpDataSource.h"
#include "avcpayloadformatread.h"
#include "avcrtpstruct.h"
#include "MccInternalDef.h"

_LIT8(AVCPSC, "\x0\x0\x0\x1");
// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::CAvcPayloadFormatRead
//
// CAvcPayloadFormatRead default constructor, can NOT contain any code,
// that might leave
// Phase #1 of 2-phase constructor
// -----------------------------------------------------------------------------
//
CAvcPayloadFormatRead::CAvcPayloadFormatRead ( ) :
    iDecoder( NULL )
    {
    iFourCC.Set( TFourCC( 'H','2','6','4' ) );
    iUsingJitterBufferExtension = ETrue;
    }


// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::ConstructL
//
// Symbian 2nd phase constructor can leave.
// Phase #2 of 2-phase constructor
// Initializes the State machine and rest of the members.
//
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatRead::ConstructL ( MDataSource* aSource )
    {
    iRtpDataSource = aSource;
    iBufferToReadExists = EFalse;

    iFourCC.Set( TFourCC( 'H','2','6','4' ) );

    // Initialize decoding state machine
    iStateMachine = CFormatDecodeStateMachine::NewL( this );
    iStateMachine->ChangeState( EDecodeIdle );
    
    iPayloadBuffer = CMMFDataBuffer::NewL(KAvcMaxRxPacketSize); 
    iFrameBuffer = CMMFDataBuffer::NewL(KAvcMaxRxPacketSize + sizeof(TRTPInfo)); 
        
    iCounter = 0;
		iCurDataSource = aSource;
		iUsingJitterBufferExtension = ETrue;
		iNalSent = 0;
    }
    
    
// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::NewL
//
// Two-phased constructor.
// Static function for creating and constructing an instance of the AVC format.
//
// Returns:  CAvcPayloadFormatRead* : pointer to created instance
// -----------------------------------------------------------------------------
//
CAvcPayloadFormatRead* CAvcPayloadFormatRead::NewL ( MDataSource* aSource )
    {
     
    if ( NULL == aSource )
        {
        User::Leave ( KErrArgument );
        }
    
    if ( KRtpSourceUID != aSource->DataSourceType( ) )
        {
        User::Leave ( KErrNotSupported );
        }

    CAvcPayloadFormatRead* self = new ( ELeave ) CAvcPayloadFormatRead();
    CleanupStack::PushL ( self );
    self->ConstructL ( aSource );
    CleanupStack::Pop ( );
    return self;
    }
    
// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::~CAvcPayloadFormatRead
//
// Destructor.
// -----------------------------------------------------------------------------
//
CAvcPayloadFormatRead::~CAvcPayloadFormatRead ( )
    {
    // commented out CreateSourceBufferL now sets aReference to ETrue.
    // datapath doesn't clean this buffer any longer.
    if ( iFrameBuffer )
        {
        delete iFrameBuffer;
        }
    if ( iPayloadBuffer)
    	delete iPayloadBuffer;
 if ( iStateMachine )
        {
        iStateMachine->Cancel( );
        delete iStateMachine;
        }
    
    delete iDecoder;
    iRtpDataSource = NULL;
    iSink = NULL;
    iCurDataSource = NULL;    

	iPayloadTypes.Close();
    }

// Virtual functions from CMMFFormatDecode    

// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::FrameTimeInterval
//
// Return the frame time interval for the given media
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CAvcPayloadFormatRead::FrameTimeInterval( TMediaId aMediaId ) const
    {
    if ( aMediaId.iMediaType == KUidMediaTypeVideo)
        {
        return TTimeIntervalMicroSeconds( 0 );  // return some correct value as required
        }
    else
        {
        return TTimeIntervalMicroSeconds( 0 );
        }
    }
// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::Streams
//
// Return the frame time interval for the given media
// -----------------------------------------------------------------------------
//
TUint CAvcPayloadFormatRead::Streams(TUid /*aMediaType*/) const
 {
 	  return 0;
 }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::Duration
//
// Return the clip duration for the given media.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CAvcPayloadFormatRead::Duration( TMediaId /*aMediaType*/ ) const
    {
    return TTimeIntervalMicroSeconds( 1000000000 );
    }
    
// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::Initialize
//
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatRead::InitializeL( )
    {
   
   		if(!iDecoder)
				iDecoder = CRFC3984Decode::NewL();
		
    }

//    The following functions are virtual function implementation from MDataSource
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::FillBufferL
// Fill Buffer
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatRead::FillBufferL( CMMFBuffer* aBuffer, 
                                         MDataSink* aConsumer, 
                                         TMediaId aMediaId )
    {
  
    if ( aMediaId.iMediaType != KUidMediaTypeVideo )
        {
        User::Leave( KErrNotSupported );
        }

    if ( aBuffer->Type( ) != KUidMmfDataBuffer )
        {
        User::Leave( KErrNotSupported );
        }

    iSink = aConsumer;
    iMediaId = aMediaId;

    iFillRequested = ETrue;

   
	if(iNalSent < iNalCount)
	{
		CAvcRtpStruct * ptr = NULL;
        TBufC8<4> buf(AVCPSC);
        
        ptr = NULL;
    //   	while((ptr = iDecoder->GetNalUnit(iNalSent++)) != NULL && iCounter < 2)
       		{
       			ptr = iDecoder->GetNalUnit(iNalSent++);
       			if(ptr)
       				{
       				iCounter++;
		        	TDes8& dataDes = STATIC_CAST( CMMFDataBuffer*, iFrameBuffer )->Data( );
		        	TPtr8 pDes = ptr->iDes->Des( );
		    			
		        	
		        	if(iUsingJitterBufferExtension)
		        	{
		        		TRTPInfo info;
		        		info.TS	= (ptr->iTimeStamp*1000)/90;
		        		info.SN = ptr->iSeqNo;
						info.DON = ptr->iDon;
						info.PT = ptr->iPacketType;
						info.FUH = ptr->iFUHeader; 	// copying FU-Header
						dataDes.Copy((TUint8*)&info, sizeof(TRTPInfo));
		    			if(ptr->iPacketType != PACKET_FU_A && ptr->iPacketType != PACKET_FU_B)
		    				dataDes.Append(AVCPSC);
		    			dataDes.Append(pDes.Ptr( ), pDes.Length( ));
		        	}
		        	else
		    		{
		    			dataDes.Copy(pDes.Ptr( ), pDes.Length( ));
		    			if(ptr->iPacketType != PACKET_FU_A && ptr->iPacketType != PACKET_FU_B)
		    				dataDes.Insert(0, AVCPSC);//Add PSC to each packet 
		    		}
		    			
		    		    			
		    		iFrameBuffer->SetTimeToPlay(TTimeIntervalMicroSeconds((ptr->iTimeStamp*1000)/90));//us
		    		iFrameBuffer->SetFrameNumber(ptr->iSeqNo);
		    			
		    		// sending first NAL unit to the jitter buffer
		    		iSink->BufferFilledL(iFrameBuffer);	
       				}
       		//	else
       		//		break;
       			
       			if(iNalSent == iNalCount)
				{
					iNalSent++;
					iDecoder->ClearBuffer();
			        iNalCount = 0;
					iStateMachine->ChangeState( EWaitSourceData ); // No payload ask for it
				}
	        }	 	
        iCounter = 0;
	}
	
	    
 
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::FillSourceBufferL
//
// Send fill buffer request to RTP Data Soure
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatRead::FillSourceBufferL( )
    {
//    iRtpDataSource->FillBufferL( iPayloadBuffer, this, iMediaId ); // RtpSourceSink doesn't really need the Media Id.  
     // RTP will fill buffer when it receives a packet      
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::SourceThreadLogon
//
// Log in to the source thread
// -----------------------------------------------------------------------------
//
TInt CAvcPayloadFormatRead::SourceThreadLogon( MAsyncEventHandler& aEventHandler )
    {
	static_cast<CMCCRtpDataSource*>(iRtpDataSource)->SetPayloadSize( 1500 );

    iCurDataSource->SourceThreadLogon( aEventHandler );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::NegotiateSourceL( MDataSink& aDataSink )
//
// Negotiate source settings to match data sink object.
// Re-size frame buffers if needed
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatRead::NegotiateSourceL( MDataSink& aDataSink )
    {
	iSink = static_cast<MDataSink*>( &aDataSink );//aDataSink;
	
	iCurDataSource->NegotiateSourceL( *this );
	

    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::SourceThreadLogoff
//
// Log out of the source thread.
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatRead::SourceThreadLogoff( )
    {
    iCurDataSource->SourceThreadLogoff( );
    }


// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::BufferFilledL
//
// Called after the data buffer is filled. Update the number of bytes read
// and the current read position for the next read operation.
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatRead::BufferFilledL( CMMFBuffer* aBuffer, 
                                           const TRtpRecvHeader& aRtpHeader )
    {
  
    User::LeaveIfNull( aBuffer );
        
    if ( KUidMmfDataBuffer != aBuffer->Type() )
        {
        User::Leave( KErrNotSupported );
        }
    
    //#ifdef TRACE_AVC_PAYLOAD_FORMAT_READ
    //        RDebug::Print ( _L ( "start CMMFAmrWbFormatRead::BufferFilledL" ) );
    //        RDebug::Print ( _L ( "M = %d, R = %d, CSI = %d, SIT = %d, PT = %d, SN = %d, CS = %d" ),
    //            aRtpHeader.iMarker, aRtpHeader.reserved, aRtpHeader.iChecksumInd,
    //            aRtpHeader.iSpeechItemToggle, aRtpHeader.iPayloadType,
    //            aRtpHeader.iSeqNum, aRtpHeader.iChecksum );
    //#endif

  
    // Copy received buffer
    iPayloadBuffer->Data().Copy( static_cast<CMMFDataBuffer*>( aBuffer )->Data() );

	iRecvHeader = aRtpHeader;
    iBufferToReadExists = ETrue;

    // Get a reference to the data
    TDes8& srcData = static_cast<CMMFDataBuffer*>( iPayloadBuffer )->Data();

	

	// Decode the payload
	iNalCount = 0;
	iDecoder->DePayloadizeFrameL(srcData, iRecvHeader.iTimestamp, iRecvHeader.iTimestamp, iRecvHeader.iSeqNum 
					, iRecvHeader.iMarker, iNalCount);            

   
    // Whenever BufferFilledL is called from RtpSourceSink
    // Data has been depayloadized, now read the data and send it to the sink (DataPath)
     iStateMachine->ChangeState( EEmptyDataToSink ); 
	 SendDataToSinkL();		// sending data to sink
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::SendDataToSinkL
//
// Send full frame buffer to Data Path
// -----------------------------------------------------------------------------
//

void CAvcPayloadFormatRead::SendDataToSinkL( )
    {
        TInt count = 0;
        CAvcRtpStruct * ptr = NULL;
        TBufC8<4> buf(AVCPSC);
        
        #ifdef TRACE_AVC_PAYLOAD_FORMAT_READ
    		RDebug::Print ( _L ( "CAvcPayloadFormatRead::SendDataToSinkL( ) - START" ) );
    		#endif
        
        iFillRequested = EFalse;
              
        
        // We give data to jitter buffer through the iSink->BufferFilledL() function below. Now when the jitter
        // buffer has handled the data, it calls back FillBufferL() and there we check if all the data is given
        // and if so, delete the buffers. Otherwise give data back to it again.
        
        iNalSent = 0;   // starting from 0 and going to iNalCount (in FillBuffer)
        
        	
       	ptr = NULL;
       	ptr = iDecoder->GetNalUnit(iNalSent++);
        if(ptr)
        {
        	TDes8& dataDes = STATIC_CAST( CMMFDataBuffer*, iFrameBuffer )->Data( );
        	TPtr8 pDes = ptr->iDes->Des( );
    			
        	
        	if(iUsingJitterBufferExtension)
        	{
        		TRTPInfo info;
        		info.TS	= (ptr->iTimeStamp*1000)/90;
        		info.SN = ptr->iSeqNo;
				info.DON = ptr->iDon;
				info.PT = ptr->iPacketType;
				info.FUH = ptr->iFUHeader; 	// copying FU-Header
				dataDes.Copy((TUint8*)&info, sizeof(TRTPInfo));
    			if(ptr->iPacketType != PACKET_FU_A && ptr->iPacketType != PACKET_FU_B)
    				dataDes.Append(AVCPSC);
    			dataDes.Append(pDes.Ptr( ), pDes.Length( ));
        	}
        	else
    		{
    			dataDes.Copy(pDes.Ptr( ), pDes.Length( ));
    			if(ptr->iPacketType != PACKET_FU_A && ptr->iPacketType != PACKET_FU_B)
    				dataDes.Insert(0, AVCPSC);//Add PSC to each packet 
    		}
    			
    		    			
    		iFrameBuffer->SetTimeToPlay(TTimeIntervalMicroSeconds((ptr->iTimeStamp*1000)/90));//us
    		iFrameBuffer->SetFrameNumber(ptr->iSeqNo);
    			
    		// sending first NAL unit to the jitter buffer
    		iSink->BufferFilledL(iFrameBuffer);
        }
        	
        
        // Clear buffers and then reset iNalCount
        //iDecoder->ClearBuffer();
        //iNalCount = 0;
            
   
    #ifdef TRACE_AVC_PAYLOAD_FORMAT_READ
    RDebug::Print ( _L ( "CAvcPayloadFormatRead::SendDataToSinkL( ) - DONE" ) );
    #endif
    }

/*void CAvcPayloadFormatRead::SendDataToSinkL( )
    {
        TInt count = 0;
        CAvcRtpStruct * ptr = NULL;
        TBufC8<4> buf(AVCPSC);
        
        #ifdef TRACE_AVC_PAYLOAD_FORMAT_READ
    		RDebug::Print ( _L ( "CAvcPayloadFormatRead::SendDataToSinkL( ) - START" ) );
    		#endif
        
        iFillRequested = EFalse;
              
        	
        for(count = 0; count < iNalCount; count++)
        {
        	ptr = NULL;
        	ptr = iDecoder->GetNalUnit(count);
        	if(ptr)
        	{
        		TDes8& dataDes = STATIC_CAST( CMMFDataBuffer*, iFrameBuffer )->Data( );
        		TPtr8 pDes = ptr->iDes->Des( );
    			
        	
        		if(iUsingJitterBufferExtension)
        		{
        			TRTPInfo info;
        			info.TS	= (ptr->iTimeStamp*1000)/90;
        			info.SN = ptr->iSeqNo;
					info.DON = ptr->iDon;
					dataDes.Copy((TUint8*)&info, sizeof(TRTPInfo));
    				dataDes.Append(AVCPSC);
    				dataDes.Append(pDes.Ptr( ), pDes.Length( ));
        		}
        		else
    			{
    				dataDes.Copy(pDes.Ptr( ), pDes.Length( ));
    				dataDes.Insert(0, AVCPSC);//Add PSC to each packet 
    			}
    			
    		    			
    			iFrameBuffer->SetTimeToPlay(TTimeIntervalMicroSeconds((ptr->iTimeStamp*1000)/90));//us
    			iFrameBuffer->SetFrameNumber(ptr->iSeqNo);
    			
    			iSink->BufferFilledL(iFrameBuffer);
        	}
        	
        }
        
        
        
        // Clear buffers and then reset iNalCount
        iDecoder->ClearBuffer();
        iNalCount = 0;
            
   
    #ifdef TRACE_AVC_PAYLOAD_FORMAT_READ
    RDebug::Print ( _L ( "CAvcPayloadFormatRead::SendDataToSinkL( ) - DONE" ) );
    #endif
    }

*/
// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::SourceDataTypeCode
//
// Return the source data type ( four CC code ) for the given media
// -----------------------------------------------------------------------------
//
TFourCC CAvcPayloadFormatRead::SourceDataTypeCode( TMediaId aMediaId )
    {
    if ( aMediaId.iMediaType == KUidMediaTypeVideo )
        {
        return iFourCC;
        }
    else
        {
        return TFourCC( ); //defaults to 'NULL' fourCC
        }
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::SetSourceDataTypeCode
//
// Set the source data type to the given four CC code for the given media
// -----------------------------------------------------------------------------
//
TInt CAvcPayloadFormatRead::SetSourceDataTypeCode( TFourCC aSourceFourCC, 
                                                   TMediaId aMediaId )
    {
   if ( KUidMediaTypeVideo != aMediaId.iMediaType ) 
        {
        return KErrNotSupported;
        }

    iFourCC = aSourceFourCC;
    iMediaId = aMediaId;

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::CreateSourceBufferL
//
// Create a source buffer for the given media and indicate in aReference if buffer
// is created.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CAvcPayloadFormatRead::CreateSourceBufferNowL( TMediaId aMediaId, 
                                                        TBool &aReference )
    {
    if ( aMediaId.iMediaType != KUidMediaTypeVideo )
        {
        User::Leave( KErrNotSupported );
        }

    // the source buffers belong to AmrPayloadFormatRead not to datapath
    // aference should be set to ETrue and destried by AmrPayloadFormatRead itself.
    aReference = ETrue;
    return iFrameBuffer; 
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::CreateSourceBufferL
//
// Create a source buffer for the given media, setting frame size to match
// the given sink buffer
// -----------------------------------------------------------------------------
//
CMMFBuffer* CAvcPayloadFormatRead::CreateSourceBufferL( TMediaId aMediaId,
                                                        //CMMFBuffer& /*aSinkBuffer*/,
                                                        TBool &aReference )
    {
    if ( aMediaId.iMediaType != KUidMediaTypeVideo )
        {
        User::Leave( KErrNotSupported );
        }
    
    return CreateSourceBufferNowL( aMediaId, aReference );
    }


// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::SinkDataTypeCode()
// Returns the datatype code for this Format Decoder
// -----------------------------------------------------------------------------
//
TFourCC CAvcPayloadFormatRead::SinkDataTypeCode( TMediaId aMediaId )
    {
    if( KUidMediaTypeVideo == aMediaId.iMediaType )
        {
        return iFourCC;
        }
    else
        {
        return TFourCC();
        }
    };

// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::CancelDlRequest( )
//
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatRead::CancelDlRequest( )
    {
  
    iStateMachine->Cancel( );
    iStateMachine->ChangeState( EDecodeIdle );
    
 
    iBufferToReadExists = EFalse;
    iFillRequested = EFalse;

    }


// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::SourcePrimeL( )
// 
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatRead::SourcePrimeL( )
    {
    iCurDataSource->SourcePrimeL();
    }


// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::SourcePlayL( )
//
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatRead::SourcePlayL( )
    {
   	iCurDataSource->SourcePlayL();
    }

// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::SourcePauseL( )
//
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatRead::SourcePauseL( )
    {
    iCurDataSource->SourcePauseL( );
    }


// -----------------------------------------------------------------------------
// CAvcPayloadFormatRead::SourceStopL( )
//
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatRead::SourceStopL( )
    {
    //CancelDlRequest( );

 
    iCurDataSource->SourceStopL( );
    }

// ---------------------------------------------------------------------------
// CAvcPayloadFormatRead::SetPayloadType
// Sets the payloadtype to be accepted.
// -----------------------------------------------------------------------------
//
void CAvcPayloadFormatRead::SetPayloadType( TUint8 aPayloadType )
    {
    iPayloadTypes.Reset();
    iPayloadTypes.Append( aPayloadType );
    }
    

// ---------------------------------------------------------------------------
// CAvcPayloadFormatRead::FillSinkBufferL
// Fill the sink buffer, not needed, currently sendDataToSinkL fills the sink buffers before sending.
// -----------------------------------------------------------------------------
//  
    
void CAvcPayloadFormatRead::FillSinkBufferL( )
{
	TFourCC();
}


void CAvcPayloadFormatRead::UseRTPInfoExtension()
{
	iUsingJitterBufferExtension = EFalse;
}

// ---------------------------------------------------------------------------

void CAvcPayloadFormatRead::ConfigurePayloadFormatL( const TDesC8& /*aConfigParams*/ )
{
	// Fill this function when integrating AVC decoder
}