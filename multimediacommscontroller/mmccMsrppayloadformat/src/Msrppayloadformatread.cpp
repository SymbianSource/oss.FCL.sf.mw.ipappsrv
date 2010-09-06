/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides a comfort noise generator class
*
*/


// INCLUDE FILES
#include <E32base.h>
#include <mmffourcc.h>
#include <mmfdatabuffer.h>
#include 	<utf.h> // charconv.lib

#include "MsrpPayloadFormatRead.h"
#include "MsrpPayloadFormatWrite.h" // TStringElement, move independent
#include "MccRtpDataSource.h"
#include "Mccinternaldef.h"
#include "MccMsrpFormatLogs.h"
#include "msrppayloadformatdefs.h"  


// ============================= LOCAL FUNCTIONS ===============================

const TUint KLostChar = 0xFFFD;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::CMsrpPayloadFormatRead
//
// CMsrpPayloadFormatRead default constructor, can NOT contain Msrp code,
// that might leave
// Phase #1 of 2-phase constructor
// -----------------------------------------------------------------------------
//
CMsrpPayloadFormatRead::CMsrpPayloadFormatRead ( ) : iFirstPacketsMissing (EFalse),
iPacketSecNumber(EFalse)
    {
    }


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatRead::ConstructL ( MDataSource* aSource )
    {
    
   	DP_MSRP_WRITE( "CMsrpPayloadFormatRead::ConstructL" );
    __ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) ); 
 
 		// we are coming downstream, the clip is now the source and
 		// datapath the sink. If we use RTP source if of source the RTPSource

    iRtpDataSource = aSource;  
    iClip = aSource;
    
    //Flags to indicate actions
	iFillRequested = EFalse;	 
	iBufferToReadExists = EFalse; 
		
    iFourCC = KMccFourCCIdMSRP ;
    //iFourCC.Set( TFourCC( 'T','1','4','0' ) );
    
    iRedData = new(ELeave)CArrayFixFlat<TStringElement>(3);
    TStringElement empty;
    iRedData->AppendL(empty);
    iRedData->AppendL(empty);
    iRedData->AppendL(empty);
		
	// TODO : refine my sizes
	iCharData = HBufC8::NewL( KDataSize );
	iDecodedBuffer = CMMFDataBuffer::NewL( KRedHeaderSize +  KDataSize );
		
    // Initialize decoding state machine 
    iStateMachine = CFormatDecodeStateMachine::NewL( this );
    iStateMachine->ChangeState( EDecodeIdle );
    
    }
    
    
// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::NewL
// Two-phased constructor.
// Static function for creating and constructing an instance of 
// the Text/Msrp Format
// Returns:  CMsrpPayloadFormatRead* : pointer to created instance
// -----------------------------------------------------------------------------
//
CMsrpPayloadFormatRead* CMsrpPayloadFormatRead::NewL(
	MDataSource* aSource )
	
    {
     
		DP_MSRP_WRITE("CMsrpPayloadFormatRead::NewL"); 

		__ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) );
 
		//__ASSERT_ALWAYS( ( KMccRtpSourceUid == aSource->DataSourceType() ||
		//           KMccMsrpSourceUid == aSource->DataSourceType() ), 
		//         User::Leave( KErrArgument ) );
		CMsrpPayloadFormatRead* self = new ( ELeave ) CMsrpPayloadFormatRead;
		CleanupStack::PushL ( self );
		self->ConstructL ( aSource );
		CleanupStack::Pop( self );
		return self;
    
    }
    
   
   
// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::~CMsrpPayloadFormatRead
//
// Destructor.
// -----------------------------------------------------------------------------
//
CMsrpPayloadFormatRead::~CMsrpPayloadFormatRead( )
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::~CMsrpPayloadFormatRead");
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
    
    if(iRedData)
        {
        delete iRedData;
        iRedData = NULL;
        }
    
    if(iCharData)
        {
        delete iCharData;
        iCharData = NULL;
        }
    
    if(iDecodedBuffer)
        {
        delete iDecodedBuffer;
        iDecodedBuffer = NULL;
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
// CMsrpPayloadFormatRead::Streams
// Return number of message streams for the given media
// -----------------------------------------------------------------------------
//
TUint CMsrpPayloadFormatRead::Streams( TUid /*aMediaType*/) const
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::Streams");
    return 1;
    }

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::FrameTimeInterval
// Return the frame time interval for the given media
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMsrpPayloadFormatRead::FrameTimeInterval( 
    TMediaId /*aMediaId*/ ) const
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::FrameTimeInterval");
        
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::
// Fill Buffer. If DataSink asks to Fill a buffer
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatRead::FillBufferL( CMMFBuffer* aBuffer, 
                                         MDataSink*  aConsumer, 
                                         TMediaId    aMediaId )
    {
	   	DP_MSRP_WRITE("CMsrpPayloadFormatRead::FillBufferL");
	    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
	    __ASSERT_ALWAYS( aBuffer->Type() == KUidMmfDataBuffer, User::Leave( KErrArgument ) );
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
//		ResetBuffers();
    }



// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::FillSinkBuffer
// Read RTP payload and convert it into Text/Msrp Data
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatRead::FillSinkBufferL()
    {
  	
  	// reset all bufs
    ResetPacketBuffers();
    const TDesC8& srcBuf( iCurrentBuffer->Data() );
    
  	DP_MSRP_WRITE2("CMsrpPayloadFormatRead::FillSinkBufferL, iPacketCount: %d ", iPacketCount );
  	DP_MSRP_WRITE2("CMsrpPayloadFormatRead::FillSinkBufferL, iFirstPacketsMissing: %d ", iFirstPacketsMissing );
  	DP_MSRP_WRITE2("CMsrpPayloadFormatRead::FillSinkBufferL, iSatisfySecNumber: %d ", iSatisfySecNumber );
  	DP_MSRP_WRITE2("CMsrpPayloadFormatRead::FillSinkBufferL, iLevelsMissing: %d ", iLevelsMissing );
  	DP_MSRP_WRITE2("CMsrpPayloadFormatRead::FillSinkBufferL, iDroppedPacketCount: %d ", iDroppedPacketCount );
    DP_MSRP_WRITE2("CMsrpPayloadFormatRead::FillSinkBufferL, iCurrentBuffer Size() : %d ", iCurrentBuffer->Data().Size() );
    DP_MSRP_WRITE2("CMsrpPayloadFormatRead::FillSinkBufferL, Currentbuffer BufferSize() : %d ", iCurrentBuffer->BufferSize() );
     
    
    // Do not forward zero length buffers, even though we filtered all unwanted data
    // allready in BufferFilledL() -method
    if ( iCurrentBuffer->BufferSize() > 0 )
        {
       
            TInt pValue(0); 
		    switch ( iCodecInfo.iRedundancyCount )  
		    {
		    		case 0:
		    			iDecodedBuffer->Data().Append( iCurrentBuffer->Data() );
		    			iSatisfySecNumber++;
		    		break;
		    		
		    		case 2:
			    		pValue = DecodePayloadL( srcBuf );
			    		User::LeaveIfError(HandleRedundancyL(pValue)); 
		    		break;
		    }
                
		if ( !pValue )
			{
			iStateMachine->ChangeState( EEmptyDataToSink );	
			}
		else
			{
	        FillSourceBufferL();
			}
        }
    else
    	{  
    	// No payload or Data sent by SipCon1, ask for more
    	DP_MSRP_WRITE("CMsrpPayloadFormatRead::FillSinkBufferL - Empty Data");
        iStateMachine->ChangeState( EWaitSourceData );                           
        }
    }

 


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::ResetBuffers()
// Send fill buffer request to RTP Data Source
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatRead::ResetBuffers()
{
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::ResetBuffers");
	if ( iCurrentBuffer ) 
		{
		iCurrentBuffer->SetStatus( EAvailable );
		iCurrentBuffer = NULL;		
		}

	if (iFrameBufferOne->Status() == EFull )
		{
		DP_MSRP_WRITE("CMsrpPayloadFormatRead::ResetBuffers() - FB1 - was FULL");
		iCurrentBuffer = iFrameBufferOne;
		iStateMachine->ChangeState( ESourceDataReady );				
		}
	else if (iFrameBufferTwo->Status() == EFull )  
		{
		DP_MSRP_WRITE("CMsrpPayloadFormatRead::ResetBuffers() - FB2 - was FULL");
		iCurrentBuffer = iFrameBufferTwo;
		iStateMachine->ChangeState( ESourceDataReady );		
		}
	else
		{
		iStateMachine->ChangeState( EWaitSourceData ); 
		} 
}


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::FillSourceBufferL
// Send fill buffer request to RTP Data Source
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatRead::FillSourceBufferL()
    { 
    	
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::FillSourceBufferL");
    __ASSERT_ALWAYS( iClip, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iPayloadBuffer, User::Leave( KErrArgument ) );

    iClip->FillBufferL(iPayloadBuffer, this, iMediaId );
    }

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::SendDataToSinkL
// Send full frame buffer to Data Path
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatRead::SendDataToSinkL( )
    {
		
	DP_MSRP_WRITE("CMsrpPayloadFormatRead::SendDataToSinkL()");
    iFillRequested = EFalse;
    iDataPath->BufferFilledL( iDecodedBuffer );
   
    }

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::CreateSourceBufferL
// Create a source buffer for the given media and indicate in aReference if 
// buffer is created.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMsrpPayloadFormatRead::CreateSourceBufferL( 
    TMediaId /*aMediaId*/, 
    TBool &aReference )
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::CreateSourceBufferL");
    // the source buffers belong to MsrpPayloadFormatRead not to datapath
    // aReference should be set to ETrue and destroyed by MsrpPayloadFormatRead 
    // itself.
    aReference = ETrue;
    iCurrentBuffer = iFrameBufferOne;
    return iFrameBufferOne; 
    
    }


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::CreateSourceBufferL
// Create a source buffer for the given media, setting frame size to match
// the given sink buffer
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMsrpPayloadFormatRead::CreateSourceBufferL(
    TMediaId aMediaId,
    CMMFBuffer& /*aSinkBuffer*/, 
    TBool &aReference )
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::CreateSourceBufferL");
    return CreateSourceBufferL( aMediaId, aReference );
    }


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::SourceDataTypeCode
// Return the source data type ( four CC code ) for the given media
// -----------------------------------------------------------------------------
//
TFourCC CMsrpPayloadFormatRead::SourceDataTypeCode( 
	TMediaId aMediaId )
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::SourceDataTypeCode");
    //TODO: Change me when platform supports
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
// CMsrpPayloadFormatRead::SetSourceDataTypeCode
// Set the source data type to the given four CC code for the given 
// media
// -----------------------------------------------------------------------------
//
TInt CMsrpPayloadFormatRead::SetSourceDataTypeCode( TFourCC aSourceFourCC, 
    											   TMediaId aMediaId )
    {
    
	//TODO Change me when platform supports
	/*
	if ( KUidMediaTypeAudio != aMediaId.iMediaType ) 
		{
		return KErrNotSupported;
		}
	*/
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::SetSourceDataTypeCode");
    iFourCC = aSourceFourCC;
    iMediaId = aMediaId;
    iClip->SetSourceDataTypeCode( iFourCC, iMediaId );
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::SourceThreadLogon
// Log in to the source thread
// -----------------------------------------------------------------------------
//
TInt CMsrpPayloadFormatRead::SourceThreadLogon( 
    MAsyncEventHandler& aEventHandler )
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::SourceThreadLogon");
   	iEventHandler = &aEventHandler;
    iClip->SourceThreadLogon( aEventHandler );
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::NegotiateSourceL( MDataSink& aDataSink )
// Negotiate source settings to match data sink object.
// Re-size frame buffers if needed
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatRead::NegotiateSourceL( MDataSink& /*aDataSink*/ )
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::NegotiateSourceL");
    __ASSERT_ALWAYS( iClip, User::Leave( KErrArgument ) );
    iClip->NegotiateSourceL( *this );
    
    }

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::SourceThreadLogoff
// Log out of the source thread.
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatRead::SourceThreadLogoff()
    {  
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::SourceThreadLogoff");
	iClip->SourceThreadLogoff( );
    }
 

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::FilterUnwantedData 
// this method can be used to filter unwanted data off
//
// e.g SipCon1 sends BOM data in first String and PS data as carriage return
// Engine should NOT touch these but Presentation layer should take care of these

// Method can be used to filtering pusposes if needed
// -----------------------------------------------------------------------------
//
TBool CMsrpPayloadFormatRead::FilterUnwantedData(const TDesC8& aBuffer)
{

	//TODO: We must do a entity recognizer, which handles this kind of data
	// This does not affect to actual formatter, instead we
	// a) must do it to application side
	// b) we must define an interface, which can be called
	TBool ret = ETrue; 
	DP_MSRP_WRITE2("CMsrpPayloadFormatRead::FilterUnwantedData() - BufferSize:%d",aBuffer.Length()); 
	
	//get Three first or Three last chars
	if( aBuffer.Length() >= 3 )
	{
		//SipCon sends BOM for the first time
		TPtrC8 pBOM = aBuffer.Left(3);
		if ( pBOM[0] == 0xEF && pBOM[1] == 0xBB && pBOM[2]== 0xBF)
		{
			//return EFalse if want to filter the whole message
			ret = ETrue;
		}
		
	
		// last are unicode PS char ( return )
		TPtrC8 pPS = aBuffer.Right(3);
		if ( pPS[0] == 0xE2 && pPS[1] == 0x80 && pPS[2]== 0xA8)
		{
			//return EFalse if want to filter
			ret = ETrue;
		}
	
	}
	//There might be back spaces and other marks
	else if( aBuffer.Length() >= 1 )
	{
		TPtrC8 pChar = aBuffer.Left(1);
		if( pChar[0] == 0x08 )
			{
				ret = ETrue;
			}
		//bell
		else if( pChar[0] == 0x07)
			{
				ret = ETrue;
			}
		//tab
		else if( pChar[0] == 0x09)
			{
				ret = ETrue;
			}
		//escape
		else if( pChar[0] == 0x1B)
			{
				ret = ETrue;
			}

		//etc
	}
	
	return ret;
}
// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::BufferFilledL
// Called after the data buffer is filled. Update the number of bytes read
// and the current read position for the next read operation.
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatRead::DataBufferFilledL( CMMFBuffer* aBuffer, 
                                           const TRtpRecvHeader& aRtpHeader )
    { 
     
     DP_MSRP_WRITE2("CMsrpPayloadFormatRead::SendDataToSinkL() - BufferSize:%d",aBuffer->BufferSize()); 
     
   	__ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aBuffer->Type() == KUidMmfDataBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iClip, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iStateMachine, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iFrameBufferOne , User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iFrameBufferTwo , User::Leave( KErrArgument ) );
		
	
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::BufferFilledL with RTP header param:");
    DP_MSRP_WRITE2("CMsrpPayloadFormatRead::BufferFilledL iPadding: %d", aRtpHeader.iPadding );
    DP_MSRP_WRITE2("CMsrpPayloadFormatRead::BufferFilledL iExtension: %d", aRtpHeader.iExtension );
    DP_MSRP_WRITE2("CMsrpPayloadFormatRead::BufferFilledL iCsrcCount: %d", aRtpHeader.iCsrcCount );
    DP_MSRP_WRITE2("CMsrpPayloadFormatRead::BufferFilledL iMarker: %d", aRtpHeader.iMarker );
    DP_MSRP_WRITE2("CMsrpPayloadFormatRead::BufferFilledL iPayloadType: %d", aRtpHeader.iPayloadType );
    DP_MSRP_WRITE2("CMsrpPayloadFormatRead::BufferFilledL iSeqNum: %d", aRtpHeader.iSeqNum );
    DP_MSRP_WRITE2("CMsrpPayloadFormatRead::BufferFilledL iTimestamp: %d", aRtpHeader.iTimestamp );
    DP_MSRP_WRITE2("CMsrpPayloadFormatRead::BufferFilledL iHeaderExtension: %d", (TInt)aRtpHeader.iHeaderExtension );
    DP_MSRP_WRITE2("CMsrpPayloadFormatRead::BufferFilledL iCsrcList: %d", (TInt)aRtpHeader.iCsrcList );
	 
	 // Do not forward smaller zero length buffers or unwanted data
    if ( aBuffer->BufferSize() > 0 && FilterUnwantedData(static_cast<CMMFDataBuffer*>( aBuffer )->Data()) )
    {
		//check seqnumber, if first packet get sequence Nr. from it
		if ( iPacketCount == 0 )
		{
			//if marker == 1, then we know this is the first packet
			if(aRtpHeader.iMarker == 1)
				{
					// flag to indicate, because first of idle sends this also
					if ( !iPacketSecNumber )
						{
						iSatisfySecNumber =	aRtpHeader.iSeqNum;
						iCheckSum = iSatisfySecNumber;
						iPacketSecNumber = ETrue;
						}
				}
			else
				{
				// first packet was missing, signal this
				iFirstPacketsMissing = ETrue;
				DP_MSRP_WRITE("First Packet Is MISSING!");
				}
				
		}
		else 
		{
			// 0, -1, -2; ( 0 means expected packet arrived )
			//iLevelsMissing = iSatisfySecNumber - aRtpHeader.iSeqNum;
            iLevelsMissing = 0; 
			DP_MSRP_WRITE2("CMsrpPayloadFormatRead::BufferFilledL iLevelsMissing: %d", iLevelsMissing );
			DP_MSRP_WRITE2("CMsrpPayloadFormatRead::BufferFilledL aRtpHeader.iSeqNum: %d", aRtpHeader.iSeqNum );
			DP_MSRP_WRITE2("CMsrpPayloadFormatRead::BufferFilledL iSatisfySecNumber: %d", iSatisfySecNumber );
		}
	
	
	iPacketCount++;
	// check for packets arriving late, redundant data allready got 
	if ( iLevelsMissing <= 0)
	{
	    // we add packet count here. This is for physical packets, not for redundant data
	   	
	    if ( iFrameBufferOne->Status() == EAvailable )
	        {
			    iFrameBufferOne->Data().Copy( static_cast<CMMFDataBuffer*>( aBuffer )->Data() );
			    iFrameBufferOne->SetTimeToPlay( aRtpHeader.iTimestamp );
				iFrameBufferOne->SetLastBuffer( aRtpHeader.iMarker );
				iFrameBufferOne->SetFrameNumber( aRtpHeader.iSeqNum );
				iFrameBufferOne->SetStatus( EFull );

	        if (iFrameBufferTwo->Status() == EAvailable )
	        	{
	        	iCurrentBuffer = iFrameBufferOne;
	        	iStateMachine->ChangeState( ESourceDataReady );
	        	}
	        }
	    else if ( iFrameBufferTwo->Status() == EAvailable )
	        {
			    iFrameBufferTwo->Data().Copy( static_cast<CMMFDataBuffer*>( aBuffer )->Data() );
			    iFrameBufferTwo->SetTimeToPlay( aRtpHeader.iTimestamp );  
				iFrameBufferTwo->SetLastBuffer( aRtpHeader.iMarker );
				iFrameBufferTwo->SetFrameNumber( aRtpHeader.iSeqNum );
				iFrameBufferTwo->SetStatus( EFull );
			
	        if (iFrameBufferOne->Status() == EAvailable ) 
		        {
		        iCurrentBuffer = iFrameBufferTwo;   
		       	iStateMachine->ChangeState( ESourceDataReady );    	
		        }
	       	}
	    else
	        {
	        // Neither of the buffers is available and leave
	        User::Leave( KErrNotReady ); 
	        }
    }
    else
		{
		DP_MSRP_WRITE("CMsrpPayloadFormatRead::BufferFilledL IGNORING Packet, data recovered!");
		}
		
    }
    else
    {
    	DP_MSRP_WRITE("CMsrpPayloadFormatRead::BufferFilledL NO PAYLOAD, Data smaller than 8 bytes ");
    	iStateMachine->ChangeState( EWaitSourceData );
    }
    
    
}



// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::DecodePayloadL( const TDesC8& aBuffer )
//
// Decodec payload according level 2 redundancy ( RFC 4103 )
// First there are Normal RTP Header, which was extracted by RTP Stack.
// Then there are two 32-bit Redundant headers and one 8 -bit primary Header.
// Then there are R2, R1 and Primary data appended respectively.
// -----------------------------------------------------------------------------
//
TInt CMsrpPayloadFormatRead::DecodePayloadL( const TDesC8& aBuffer )
{	
		DP_MSRP_WRITE("CMsrpPayloadFormatRead::DecodePayload()");
		
		TInt ret = KErrNone;
		RDebug::Print( _L("CMsrpPayloadFormatRead::DecodePayload() -> %d") , aBuffer.Size() );
		const TUint8* aPtr = aBuffer.Ptr(); 
		TInt ctr(0);
	
		// get 1st 32 = R2 header
		TUint32 r2Header = Read32( aPtr );
		
		RDebug::Print( _L("CMsrpPayloadFormatRead R2 Header Data : %x"), r2Header );
		aPtr += 4;
		ctr+= 4; 
		TUint16 aR2Lenght(0), aR2TimeStamp(0);
		TInt err = ParseRedHeader( r2Header, aR2Lenght, aR2TimeStamp );
		
	    RDebug::Print( _L("CMsrpPayloadFormatRead R2 Lengt : %d , TimeStamp %x "), aR2Lenght, aR2TimeStamp );
		if( err )
		{
			User::Leave( err );
		}
		
		// get 2nd 32 =  R1 header
		TUint32 r1Header = Read32( aPtr );
		RDebug::Print( _L("CMsrpPayloadFormatRead R1 Header Data : %x"), r1Header );
		aPtr += 4;
		ctr +=4; 
		TUint16 aR1Lenght(0), aR1TimeStamp(0);
		err = ParseRedHeader( r1Header, aR1Lenght, aR1TimeStamp );
		RDebug::Print( _L("CMsrpPayloadFormatRead R1 Lengt : %x , TimeStamp %x "), aR1Lenght, aR1TimeStamp );
		if( err )
		{
			User::Leave( err );
		}
		
		// get 
		TUint8 aPt = ( Read8( aPtr ) & 255 ); 
		aPtr += 1;
		ctr += 1;
		TUint8 pPayloadType = ( aPt & 127 ); 
		TUint8 pBit = ( aPt >> 7 );
	
		RDebug::Print( _L("CMsrpPayloadFormatRead Primary data : %d "), aPt );
		RDebug::Print( _L("CMsrpPayloadFormatRead Primary bit : %d , PT %d "), pBit, pPayloadType );
		
		//TODO: When IOP-Tests Are done clean me out.
		TPtr8 pP = iCharData->Des();
		pP.FillZ();
		pP.Zero();
		pP.Append(aPtr, aR2Lenght); 
		aPtr += aR2Lenght;
		TBuf16<KDataSize> unicode;
	  	User::LeaveIfError(
	    			CnvUtfConverter::ConvertToUnicodeFromUtf8(unicode, pP ));
		TStringElement data;
		data.iData = unicode;
		iRedData->AppendL(data);
		
		RDebug::Print( _L("CMsrpPayloadFormatRead GOT Data %S "), &pP );
		
		pP.FillZ();
		pP.Zero();
		pP.Append(aPtr, aR1Lenght);
		aPtr += aR2Lenght;
		unicode.FillZ();
		unicode.Zero();
		User::LeaveIfError(
	    			CnvUtfConverter::ConvertToUnicodeFromUtf8(unicode, pP ));
		data.iData = unicode;
		iRedData->AppendL(data);

		RDebug::Print( _L("CMsrpPayloadFormatRead GOT Data %S "), &pP );
		TInt rest = aBuffer.Size() - ( ctr + aR2Lenght + aR1Lenght);
		
		//NO PRIMARY DATA, means redundant packet 
		if( rest == 0)
		{
			ret = KErrNotFound;	
		}
		
		RDebug::Print( _L("CMsrpPayloadFormatRead Rest %d "), rest );
		
		pP.FillZ();
		pP.Zero();
		pP.Append(aPtr, rest );
		aPtr += aBuffer.Size();
		unicode.FillZ();
		unicode.Zero();
		User::LeaveIfError(
	    			CnvUtfConverter::ConvertToUnicodeFromUtf8(unicode, pP ));
		data.iData = unicode;
		iRedData->AppendL(data);
		
		RDebug::Print( _L("CMsrpPayloadFormatRead GOT Data %S "), &pP );
		return ret;
	
}


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::ParseRedHeader
// Small helper method that parses the 32-bit redundant header
// according to RFC 4103
// -----------------------------------------------------------------------------
//
TInt CMsrpPayloadFormatRead::ParseRedHeader(TUint32 /*aHeader*/, TUint16& /*aLenght*/, TUint16& /*aTimeStamp*/ )
{
	// Not Supported
#if 0
		TUint8 aPt = ( aHeader >> 24 ) & 255;
		TUint8 aTest = ( aHeader ) & 255;
		
		TUint8 aBit = ( aPt >> 7 ) & 1;
		TUint8 aPayloadType = ( aPt & 127 );
		//TODO: Checks fi PayloadTypes and bits match
		
		aTimeStamp = ( aHeader >> 10 ) & 0x3FFF; 
		aLenght = ( aHeader & 0x3FF );
		//TODO: Checks if timestampoffset and lenght is ok
#endif
		return KErrNone;
}


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::HandleRedundancyL
// Logic method, that updates all counters and decides what data we should
// append to userBuffer. If packets are missing, we must use redundant data
// -----------------------------------------------------------------------------
//
TInt CMsrpPayloadFormatRead::HandleRedundancyL(TInt aErrNo)
{
		
		DP_MSRP_WRITE2("CAnyPayloadFormatRead::BufferFilledL iLevelsMissing: %d", iLevelsMissing );
		DP_MSRP_WRITE2("CAnyPayloadFormatRead::BufferFilledL iFirstPacketsMissing: %d", iFirstPacketsMissing );
		DP_MSRP_WRITE2("CMsrpPayloadFormatRead::HandleRedundancy() -> 0 :%S", &iRedData->At(0).iData  );
		DP_MSRP_WRITE2("CMsrpPayloadFormatRead::HandleRedundancy() -> 1 :%S", &iRedData->At(1).iData  );
		DP_MSRP_WRITE2("CMsrpPayloadFormatRead::HandleRedundancy() -> 2 :%S", &iRedData->At(2).iData );
		
			//if first packet is missing handle all redData
			if( iFirstPacketsMissing )
			{	
	        	iDecodedBuffer->Data().Append( iRedData->At(2).iData );
	        	iDecodedBuffer->Data().Append( iRedData->At(1).iData );
	        	iDecodedBuffer->Data().Append( iRedData->At(0).iData );					
				iSatisfySecNumber += 3;
			}
			else // check levels from 
			{
				switch(iLevelsMissing)
				{	
						case(0):		
							// expected packet errived
							iDecodedBuffer->Data().Append(iRedData->At(2).iData);
							iSatisfySecNumber ++;	
						break;
						
						case(-1): // one packet missing
							if( aErrNo )
								{
								iDecodedBuffer->Data().Append(iRedData->At(2).iData);	
								}
							if ( iRedData->At(1).iData.Length() != 0 )
								{
								iDecodedBuffer->Data().Append(iRedData->At(1).iData);	
								}
							iSatisfySecNumber += 2;
							iDroppedPacketCount += 1;	
						break;
						
						case(-2): // two packets missing
							if( aErrNo )
								{
								iDecodedBuffer->Data().Append(iRedData->At(2).iData);	
								}
							if ( iRedData->At(1).iData.Length() != 0 )
								{
								iDecodedBuffer->Data().Append(iRedData->At(1).iData);	
								}
							if ( iRedData->At(0).iData.Length() != 0 )
								{
								iPayloadBuffer->Data().Append(iRedData->At(0).iData);	
								}
							iSatisfySecNumber += 3;
							iDroppedPacketCount += 2;
						break;
						 
						default:
							//bacause if network idles or in debugging state
							// a lot of packets might not be received.
							//we are handlng all as missing chars
							DP_MSRP_WRITE("CMsrpPayloadFormatRead::LevelsMissing > 2!");
						
							iDecodedBuffer->Data().Append(KLostChar);
							iSatisfySecNumber += (-iLevelsMissing ) +1;
							iDroppedPacketCount += (-iLevelsMissing );
						
						break;
				}
			}		
	return KErrNone;
}
        
         

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::ResetPacketBuffers()
// Helper method that that resets all buffers used
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatRead::ResetPacketBuffers()
	{
	DP_MSRP_WRITE("CMsrpPayloadFormatRead::ResetPacketBuffers()");
	iRedData->Reset();
	iCharData->Des().FillZ();
	iCharData->Des().Zero();
	iDecodedBuffer->Data().FillZ();
	iDecodedBuffer->Data().Zero();
	}


// NOT CALLED, BECAUSE WE ARE USING RTP ONLY
void CMsrpPayloadFormatRead::BufferFilledL( CMMFBuffer* /*aBuffer*/ )
	{        
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::BufferFilledL");
    User::Leave( KErrNotSupported );
	}
	
	
// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::Duration
// Return the clip duration for the given media.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMsrpPayloadFormatRead::Duration( 
	TMediaId /*aMediaType*/ ) const
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::Duration");    
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }



// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::LastDlPacketCount
// Return current packet count for current talk spurt
// -----------------------------------------------------------------------------
//
TInt CMsrpPayloadFormatRead::LastDlPacketCount( ) 
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::LastDlPacketCount");   
    return iPacketCount;
    }
 


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::CancelDlRequest( )
// 
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatRead::CancelDlRequest()
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::CancelDlRequest");
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
    iCurrentBuffer = iFrameBufferOne;
    iFillRequested = EFalse;
    
    }

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::SourcePrimeL()
// 
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatRead::SourcePrimeL()
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::SourcePrimeL");
    __ASSERT_ALWAYS( iClip, User::Leave( KErrArgument ) );
    iClip->SourcePrimeL();
    }


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::SourcePlayL()
//
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatRead::SourcePlayL()
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::SourcePlayL");
    __ASSERT_ALWAYS( iClip, User::Leave( KErrArgument ) );

		// init packet count
    iPacketCount = 0;
    
    //init satisfiable sec number count
    iSatisfySecNumber = 0;
    
    iCheckSum = 0;
    
    // init levelsMissingNumber
    iLevelsMissing = 0;
    
    //init flag
    iPacketSecNumber = EFalse;
    
    iClip->SourcePlayL();
    
    }

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::SourcePauseL()
//
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatRead::SourcePauseL()
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::SourcePauseL");
    __ASSERT_ALWAYS( iClip, User::Leave( KErrArgument ) );
    
    this->CancelDlRequest();
    iClip->SourcePauseL();
    
    }

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::SourceStopL( )
//
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatRead::SourceStopL()
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::SourceStopL");
    __ASSERT_ALWAYS( iClip, User::Leave( KErrArgument ) );
    
    this->CancelDlRequest();
    iClip->SourceStopL();
    
    }

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::SinkDataTypeCode()
// Returns the datatype code for this Format Decoder
// -----------------------------------------------------------------------------
//
TFourCC CMsrpPayloadFormatRead::SinkDataTypeCode( TMediaId aMediaId )
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::SinkDataTypeCode");
    //TODO: We have to check that media type text
	
		if ( KUidMediaTypeAudio == aMediaId.iMediaType )
			{
			return iFourCC;
			}
		else
			{
			return TFourCC(); //defaults to 'NULL' fourCC
			}
	
    }
   
// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::ConfigurePayloadFormatL
// Configure payload decoding parameters.
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatRead::ConfigurePayloadFormatL( 
	const TDesC8& aConfigParams )
	
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::ConfigurePayloadFormatL");
    TMccCodecInfoBuffer infoBuffer;
    infoBuffer.Copy( aConfigParams );
    TMccCodecInfo cInfo = infoBuffer();
    
    if ( cInfo.iIsUpdate && cInfo.iFrameSize == iCodecInfo.iFrameSize )
        {
        //__MsrpFORMAT_CONTROLL( "CMsrpPayloadFormatRead::ConfigurePayloadFormatL, nothing to configure" )
        return;
        }
        
    UpdateConfigurationL( aConfigParams );
        
}


void CMsrpPayloadFormatRead::UpdateConfigurationL(const TDesC8& aConfigParams)
	{
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::UpdateConfigurationL");
	TMccCodecInfoBuffer infoBuffer;
    infoBuffer.Copy( aConfigParams );
    TMccCodecInfo cInfo = infoBuffer();
	
	//TODO: get me right 
   // iCodecInfo = cInfo;
   //	iCodecInfo.iRedundancyCount = 0;
   iCodecInfo = cInfo;

    //Create buffers respectively
  	if ( !iFrameBufferOne )
	    {
	    iFrameBufferOne = 
	    	CMMFDataBuffer::NewL( KRedHeaderSize + KDataSize );
	    iFrameBufferOne->SetStatus( EAvailable );
	    }
	    
	if ( !iFrameBufferTwo )
	    {
	    iFrameBufferTwo =
	    	CMMFDataBuffer::NewL( KRedHeaderSize + KDataSize );
	    iFrameBufferTwo->SetStatus( EAvailable );
	    }
	    
	if ( iPayloadBuffer && iSourceBufOwnership ) 	
	    {
	    delete iPayloadBuffer;
	    iPayloadBuffer = NULL;
	    }
    
    iPayloadBuffer 
        = CreateClipBufferL( KRedHeaderSize + KDataSize, iSourceBufOwnership );
    
    iCurrentBuffer = NULL;
    
	}
// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::CreateClipBufferL
// Creates buffer needed in data transfer with format readers clip.
// -----------------------------------------------------------------------------
//
CMMFDataBuffer* CMsrpPayloadFormatRead::CreateClipBufferL( 
        TUint aSize, TBool& aIsOwnBuffer )
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::CreateClipBufferL");
    CMMFDataBuffer* buffer( NULL );
    
    if ( iClip->CanCreateSourceBuffer() )
        {
        static_cast<CMMFFormatDecode*>( iClip )->SuggestSourceBufferSize( aSize );
        TBool reference( EFalse );
        
        //TODO: We have to change media type
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

// ==========================    HELPER FUNCTIONS    =========================
// ========================== 						 =========================
// ========================== 						 =========================


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::Read32
// Helper method that reads 32-bit unsigned integer from databuffer
// -----------------------------------------------------------------------------
//
TUint32 CMsrpPayloadFormatRead::Read32( const TUint8* const aPointer )
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::Read32");
    return ( aPointer[3] +
             ( static_cast<TUint32>( aPointer[2] ) << 8 ) +
             ( static_cast<TUint32>( aPointer[1] ) << 16 ) +
             ( static_cast<TUint32>( aPointer[0] ) << 24 ) );
    } 
    
// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::Read8
// Helper method that reads 8-bit unsigned integer from databuffer
// -----------------------------------------------------------------------------
TUint32 CMsrpPayloadFormatRead::Read8( const TUint8 *const aPointer )
	{	
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::Read8");
		
    return static_cast<TUint32> ( aPointer[0] );
	}
	
// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::Read24
// Helper method that reads 24-bit unsigned integer from databuffer
// -----------------------------------------------------------------------------
TUint32 CMsrpPayloadFormatRead::Read24( const TUint8 *const aPointer )
	{
    DP_MSRP_WRITE("CMsrpPayloadFormatRead::Read24");
    
	return ( aPointer[2] + ( static_cast<TUint32>( aPointer[1] ) << 8 ) +
	       ( static_cast<TUint32>( aPointer[0] ) << 16 ) );
	}
	
	    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
