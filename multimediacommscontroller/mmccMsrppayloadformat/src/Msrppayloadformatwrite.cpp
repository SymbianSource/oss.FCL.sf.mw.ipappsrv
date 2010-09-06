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
* Description:    Provides a comfort noise generator class
*
*/

// INCLUDE FILES

#include    <rtpheader.h> 
#include 	<e32cmn.h>
#include 	<utf.h> // charconv.lib
 
#include    "msrppayloadformatwrite.h" 
#include    "mccrtpdatasink.h"
#include 	"Mccrtpinterface.h"

#include    "mccmsrpformatlogs.h"
#include    "mccuids.hrh"
#include    "mccdef.h"
#include    "mccrtpmediaclock.h"
#include    "mccinternaldef.h"
#include 	"msrppayloadformatdefs.h"
#include    "mmccinterfacedef.h"
#include    "mccmsrpsink.h"


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatRead::CMsrpPayloadFormatRead
// C++ default constructor can NOT contain any code, that
// might leave.
// 
// iIdleMode is set to ETrue, because when starting we are in IdleMode
// -----------------------------------------------------------------------------
//
CMsrpPayloadFormatWrite::CMsrpPayloadFormatWrite(): iIdleMode(ETrue)
    {    
    }
    
// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatWrite::ConstructL ( MDataSink* aSink )
    {
     
    DP_MSRP_WRITE("CMsrpPayloadFormatWrite::ConstructL");
    
    // Set default values
    iFourCC = KMccFourCCIdMSRP ;
    //iFourCC.Set( TFourCC( 'T','1','4','0' ) );

	//we are going upstream, so our source is the datapath and
	// sink is the clip. If we use RTP, which  we do, 
	// our sink is of cource the RTPSink
    iClip = aSink;    
    iIsRtpSink = ( KMccRtpSinkUid  == aSink->DataSinkType() );
    iIsMsrpSink = (KMccMsrpSinkUid == aSink->DataSinkType() );
    
    // two levels of redundancy is allways assumed, not necessarily used
    // RFC4103 RECOMMENDED
    iRedData = new(ELeave)CArrayFixFlat<TStringElement>(3);
    
    //userBuffer, handles all data put in by user
    iUserBuffer = new(ELeave)CArrayFixFlat<TStringElement>(3);
    
    // init data to null
    TStringElement one;
    TStringElement two;
    TStringElement three;
    iRedData->AppendL(one);
    iRedData->AppendL(two); 
    iRedData->AppendL(three);
     
    // maxsize of the packet
    iBuf = HBufC8::NewL( KRedHeaderSize + KDataSize );
    iData = const_cast<TUint8*>( iBuf->Des().Ptr() );
  	
    if ( iIsRtpSink )
        {
        CMccRtpDataSink* tmp = static_cast<CMccRtpDataSink*>( aSink );
        iRtpDataSink = static_cast<MMccRtpDataSink*>( tmp );
        }
    else if(iIsMsrpSink)
        {
        CMccMsrpSink *tmp = static_cast<CMccMsrpSink*>( aSink );
        iMsrpDataSink = static_cast<CMccMsrpSink*>( tmp );
        }
    else
        {
    	DP_MSRP_WRITE("CMsrpPayloadFormatWrite::ConstructL Leaving: No RTPSink!");
    	User::Leave(KErrNotSupported);	
        }

    // Initialize state machine
    iStateMachine = CFormatEncodeStateMachine::NewL( this );
    
    //init state machine to idle state
    iStateMachine->ChangeState( EEncodeIdle );
    
    // Construct Timer for redundant generations.
    iTimer = CSendStateMachine::NewL( this ); 
    
    }

    
// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMsrpPayloadFormatWrite* CMsrpPayloadFormatWrite::NewL ( MDataSink* aSink )
    {
 	
 	
 	DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::NewL" );
    __ASSERT_ALWAYS( aSink, User::Leave( KErrArgument ) );
    
    CMsrpPayloadFormatWrite* self = new (ELeave) CMsrpPayloadFormatWrite;
    CleanupStack::PushL( self );
    self->ConstructL( aSink );
    CleanupStack::Pop( self );
    return self;
    
    }

// Destructor 
CMsrpPayloadFormatWrite::~CMsrpPayloadFormatWrite ()
    {
    
    if ( iStateMachine )
        {
        iStateMachine->Cancel();
        delete iStateMachine;
        }
    
    if(iTimer) 
	    {
	    	iTimer->Cancel();
	    	delete iTimer;	
	    }
	   
    
    iRtpMediaClock = NULL;
    iClip = NULL;
    
    delete iSourceBuffer;
    delete iSinkBuffer;
    delete iRedData;
    delete iUserBuffer;
    delete iBuf;
   
    }


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::SinkThreadLogon
// Passes the logon command to the sink clip
// -----------------------------------------------------------------------------
//
TInt CMsrpPayloadFormatWrite::SinkThreadLogon(MAsyncEventHandler& aEventHandler)
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatWrite::SinkThreadLogon");
    iClip->SinkThreadLogon( aEventHandler );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::SinkThreadLogoff
// Passes the logoff command to the sink clip
// -----------------------------------------------------------------------------
void CMsrpPayloadFormatWrite::SinkThreadLogoff()
    {   
    DP_MSRP_WRITE("CMsrpPayloadFormatWrite::SinkThreadLogoff");
    iClip->SinkThreadLogoff();
    }


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::CreateSinkBufferL
// Create a sink buffer
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMsrpPayloadFormatWrite::CreateSinkBufferL( TMediaId /*aMediaId*/,
                                                        TBool &aReference )
    { 
    	
   	DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::CreateSinkBufferL" );

	/* TODO: Remove me when platform supports
    if ( KUidMediaTypeText != aMediaId.iMediaType ) 
    {
    User::Leave( KErrNotSupported ); 
    }
    */    
    aReference = ETrue;
    return CreateSinkBufferOfSizeL( KMaxRTTChunkSize + KRedHeaderSize );
    
    }

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::CreateSinkBufferOfSizeL
// Create a sink buffer of the given size.
// -----------------------------------------------------------------------------
//
CMMFDataBuffer* CMsrpPayloadFormatWrite::CreateSinkBufferOfSizeL( TUint aSize )
    {
	
	DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::CreateSinkBufferOfSizeL" );
  
    if ( !iSourceBuffer )
	    {
	    iSourceBuffer = CMMFDataBuffer::NewL( aSize );
    	iSourceBuffer->Data().FillZ( aSize );
    	iSourceBuffer->SetRequestSizeL( aSize );
	    }
	    
    return iSourceBuffer;
    
    }

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::EmptyBufferL
// Empty the given source buffer
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatWrite::EmptyBufferL( CMMFBuffer* aBuffer, 
                                            MDataSource* aSupplier, 
                                            TMediaId /*aMediaId*/ )
    {
    
    DP_MSRP_WRITE("CMsrpPayloadFormatWrite::EmptyBufferL");
  
    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS ( aBuffer == iSourceBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aSupplier, User::Leave( KErrArgument ) );
    
   	// TODO: Remove me when platform supports
	// __ASSERT_ALWAYS( KUidMediaTypeAudio == aMediaId.iMediaType,
	// User::Leave( KErrNotSupported ) );
    
    // Cancel timer, so timer does not interrupt our current sending.
    // Timer enables empty blocks to be appended when redundancy is on.
    iTimer->Cancel();
       
    iDataPath = aSupplier;
    iSourceBuffer = static_cast<CMMFDataBuffer*>( aBuffer );
    
    if ( !iSinkBuffer )
        {        
        iStateMachine->ChangeState( ESourceBufferEmptied );
        return;
        }
        
	if ( !iSinkBuffer->Data().Length() ) 
	    {
		//User::LeaveIfError( iRtpMediaClock->GetTimeStamp( iKey, iCurrentTimestamp ) );
   		iSinkBuffer->SetTimeToPlay( TInt64( iCurrentTimestamp ) );
   		iSinkBuffer->SetFrameNumber( aBuffer->FrameNumber() );	
	    }
       
       
      // We signal StateMachine to ChangeState() 
      // this calls back EmptySourceBufferL 
      iStateMachine->ChangeState( EEmptySourceBuffer );
   
    } 


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::SetSinkDataTypeCode
// Set the sink data type to the given four CC code for the given media
// -----------------------------------------------------------------------------
//
TInt CMsrpPayloadFormatWrite::SetSinkDataTypeCode( TFourCC aSinkFourCC,
                                                   TMediaId /*aMediaId*/ )
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatWrite::SetSinkDataTypeCode");
	/* TODO: Remove me when platform supports
    if ( KUidMediaTypeText != aMediaId.iMediaType )
    {
    User::Leave( KErrNotSupported );
    }
    */
    iFourCC = aSinkFourCC;
    return KErrNone;
    
    }


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::SinkDataTypeCode
// Return the sink data type (four CC code) for the given media ID
// -----------------------------------------------------------------------------
//
TFourCC CMsrpPayloadFormatWrite::SinkDataTypeCode( TMediaId /*aMediaId*/ )
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatWrite::SinkDataTypeCode");
  	/* TODO: Remove me when platform supports
    if ( KUidMediaTypeText == aMediaId.iMediaType )
        {
        return iFourCC;
        }
    else
        {
        return TFourCC(); //defaults to 'NULL' fourCC
        }
        */
	return iFourCC;
        
    }

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::BufferEmptiedL
// Called after payload buffer is completely emptied by RtpDataSink.
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatWrite::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
    {
    DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::BufferEmptiedL" );
	// nothing to be done
	
	//NOTE: 14.12.2007 - LeaveScan causes warning, but this is a interface method
    }


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::FrameTimeInterval
// Return the frame time interval for the given media
// Return allways 0
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds 
    CMsrpPayloadFormatWrite::FrameTimeInterval( TMediaId /*aMediaId*/ ) const
    {
    DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::FrameTimeInterval" );
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::Duration
// Return the frame time interval for the given media
// NOT SUPPORTED
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds 
    CMsrpPayloadFormatWrite::Duration( TMediaId /*aMediaType*/ ) const
    {
    DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::Duration" );
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::EmptySourceBufferL
// Empty the given sourcebuffer
// Sourcebuffer is given in iSourceBuffer
// Called by StateMachine
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatWrite::EmptySourceBufferL()
	{
				
	  	DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::EmptySourceBufferL" );
	 
	  	const TDesC8& srcDes( iSourceBuffer->Data() );
    	const TUint8* srcPtr = srcDes.Ptr();
  		TInt length = iSourceBuffer->Data().Length();
  		
  		if ( iSourceBuffer->Data().Length() ) 
	  		{
	  		
		  		if( KMaxRTTChunkSize < length )
				  	{
				  		DP_MSRP_WRITE( "ERR - RTT Payload Length exceeded MaxChunkSize" );
				  		User::Leave(KErrArgument);
				  	}
			  	
		  		TBuf16<KMaxRTTChunkSize> unicode;
		    	User::LeaveIfError(
		    			CnvUtfConverter::ConvertToUnicodeFromUtf8(unicode, TPtrC8(srcPtr)));			
					
				//buffer the user data
				TStringElement buffer;
				buffer.iData = unicode;
				iUserBuffer->AppendL(buffer);
				
				FormUserDataL();
				ForwardDataL();
					
			}
}   


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::TimerExpiredL()
// Sends Redundant data after buffer period, if no user data 
// appeared before that.
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatWrite::TimerExpiredL()
{		
	DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::TimerExpiredL" );
	
	// Because timer expired, we must send redundant data anyway
	TStringElement nullData;
	iUserBuffer->AppendL(nullData);
	
	FormUserDataL();
	//TODO:Handle other Errors if any
	TInt err = SendRedundantDataL();
	if ( err != KErrNone )
		{
			GotoIdleL(); 
		}
}
 

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::ForwardDataL()
// Sends Redundant data after buffer period, if no user data 
// appeared before that.
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatWrite::ForwardDataL()
{
    DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::ForwardDataL" );
    
    // Just Send Data
    TInt err = SendRedundantDataL();
    
    if ( err == KErrNone )
        {
            iStateMachine->ChangeState( ESourceBufferEmptied ); 
        }
} 



// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::SendRedundantDataL()
// A Method that sends the redundant buffer to Sink
// Calls CMsrpPayloadFormatWrite::DeliverPacketL
// -----------------------------------------------------------------------------
//
TInt CMsrpPayloadFormatWrite::SendRedundantDataL()
	{
    DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::SendRedundantDataL" );
    
	TStringElement zero = iRedData->At(0);
	TStringElement one = iRedData->At(1);
	TStringElement two = iRedData->At(2);
 	
 	DP_MSRP_WRITE( "******************");
 	DP_MSRP_WRITE2( "SendRedundantDataL - Zero Lenght: %d", zero.iData.Length() ); 
 	DP_MSRP_WRITE2( "SendRedundantDataL - R1 Lenght: %d", one.iData.Length() );
 	DP_MSRP_WRITE2( "SendRedundantDataL - R2 lenght: %d", two.iData.Length() );
 	
	if ( zero.iData.Length() == 0 && one.iData.Length() == 0 && two.iData.Length() == 0 )
	 {
	 	DP_MSRP_WRITE( "ERR - SendRedundantDataL - All Data is 0 lenght returning, KErrArgument!"); 
	 	return KErrArgument;
	 }
		 
	TDes8& destDes( iSinkBuffer->Data() );
	
 	// Assemble redundant data
    AssembleRedPacket();
    // Append current data to buffer
    TPtr8 ptr = iBuf->Des();
    destDes.Append(  ptr );
    
    DP_MSRP_WRITE2( "CMsrpPayloadFormatWrite HeapData size: %d", ptr.Size() );
    DP_MSRP_WRITE2( "CMsrpPayloadFormatWrite HeapData Length: %d", ptr.Length() );
    DP_MSRP_WRITE ("CMsrpPayloadFormatWrite::DeliverPacketL, Packet Assembled" );
	DP_MSRP_WRITE( "******************");
	
    DeliverPacketL( *iSinkBuffer );	
	return KErrNone;
		
	}


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::FormUserDataL()
//
// A function that refreshes the RedundantData Buffer, so that
// We just can send data that is there.
// -----------------------------------------------------------------------------
//
TInt CMsrpPayloadFormatWrite::FormUserDataL()
{
    DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::SendRedundantDataL" );
	
    TStringElement primary;

    // if there is data
    if ( iUserBuffer->Count() )
        {
            primary = iUserBuffer->At(0);
        }
        
    //set timestamp to primary data, this is used to calculate offset
    primary.iTimestamp = iSinkBuffer->TimeToPlay().Int64();
    
    // can now reset user buffer
    iUserBuffer->Reset();
    
    // get current redundants
    TStringElement zero = iRedData->At(0);
    TStringElement one = iRedData->At(1); 
    
    // we can now reset old redundant also
    iRedData->Reset();
    
    // re-order redundant data, this must be always present, 
    // if redundancy changes on-fly from e.g 0 to 2
    iRedData->AppendL(primary);
    iRedData->AppendL(zero);
    iRedData->AppendL(one);
    
    return KErrNone;
}


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::GoToIdleL()
//
// When there is no data to send, we must go to idle mode
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatWrite::GotoIdleL()
	{
	
    DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::GoToIdleL()");
    //reset redundancy buffer
    iRedData->Reset();
    //add default data  
    TStringElement empty;
    iRedData->AppendL(empty);
    iRedData->AppendL(empty);
    iRedData->AppendL(empty);
    iIdleMode = ETrue;
    
    //Set State that we are waiting for Data
    iStateMachine->ChangeState( EWaitForSourceData );
    
    // Cancel the Timer also
    iTimer->Cancel();
		 	
	}
 

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::AssembleRedPacket()
//
// In AssembleRedPacket we must assemble right kind of
// payload. We switch between redundant and plain data.
// If redundancy is used a redundant datapacket is generated!
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatWrite::AssembleRedPacket()
	{
    DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::AssembleRedPacket" );
    switch( iCodecInfo.iRedundancyCount )
    {
        case 0:
            // just append the primary data and no headers
            iBuf->Des().Append( iRedData->At(0).iData );
        break;
        
        case 2:
             TInt err = GenerateRedPacket(); 
             //TODO: Handle Errors in Creating packet
        break;
        
    }

	}

	
// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::GenerateRedPacket() 
//
// In GenerateRedPacket we must assemble right kind of
// payload according to RFC 4103. Level 2 redundancy is used.
// -----------------------------------------------------------------------------
//
TInt CMsrpPayloadFormatWrite::GenerateRedPacket() 
{
		
		DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::GenerateRedPacket()" );
		
		TStringElement primary = iRedData->At(0);
		TStringElement r1 = iRedData->At(1);
		TStringElement r2 = iRedData->At(2);

		DP_MSRP_WRITE( "*******************************");
		DP_MSRP_WRITE2("CMsrpPayloadFormatWrite::GenerateRedPacket PR Length: %d", primary.iData.Length() );
		DP_MSRP_WRITE2("CMsrpPayloadFormatWrite::GenerateRedPacket R1 Length: %d", r1.iData.Length() );
		DP_MSRP_WRITE2("CMsrpPayloadFormatWrite::GenerateRedPacket R2 Length: %d", r2.iData.Length() );

		// pointer to data
		TUint8* dataP = iData;
		TInt count(0);
		//Fill Data with binary zeroes and zero old
		Mem::FillZ( dataP, KRedHeaderSize + KDataSize ); 
		iBuf->Des().Zero();
    
    	
		/*************
		data[0] = 8 bit
		1 bit - redundancy marker bit 1
		7 bit - payloadtype
		***************/
		
		iData[0] |= ( 1 << 7 );
		iData[0] |= iCodecInfo.iRedundantPayload; 
		//iData[0] |= 106; 
		dataP += 1;
		count ++;
			
			
		/**************
		data[1-3] = 24 bit
		14 bit - timestamp offset of R2
		10 bit - length of  R2
		***************/
		
		TUint32 temp(0);
		temp |= ( SetTimeOffset(primary.iTimestamp, r2.iTimestamp ) << 10  ); 
		
		TUint16 size(r2.iData.Length()); 
		temp |= size; 
			
		Write24(dataP, temp); 
		dataP += 3;
		count += 3; 
		
		DP_MSRP_WRITE2( "CMsrpPayloadFormatWrite::GenerateRedPacket DATA[0]: %x", iData[0] );
		DP_MSRP_WRITE2( "CMsrpPayloadFormatWrite::GenerateRedPacket DATA[1]: %x", iData[1] );
		DP_MSRP_WRITE2( "CMsrpPayloadFormatWrite::GenerateRedPacket DATA[2]: %x", iData[2] );
		DP_MSRP_WRITE2( "CMsrpPayloadFormatWrite::GenerateRedPacket DATA[3]: %x", iData[3] );
		
			
		/**************
		data[4] = 8 bit
		1 bit - redundancy marker bit 1
		7 bit - payloadtype
		***************/
		
		iData[4] |= ( 1 << 7 );
		iData[4] |= iCodecInfo.iRedundantPayload;
		//iData[4] |= 106;
		dataP += 1; 
		count ++;
	
		/**************
		data[5-7] = 24 bit
		14 bit - timestamp offset of R1
		10 bit - length of  R1
		***************/			
		
		temp = 0;
		temp |= ( SetTimeOffset(primary.iTimestamp, r1.iTimestamp ) << 10  );
	
		size = r1.iData.Length();  
		temp |= size; 
			
		Write24(dataP, temp);
		dataP += 3;
		count += 3;
				
		/**************
		data[8] = 8 bit
		1 bit - normal marker bit 0
		7 bit - payloadtype 
		***************/
		iData[8] |= ( 0 << 7 );
		iData[8] |= iCodecInfo.iPayloadType;
		//iData[8] |= 105;
		dataP += 1;
		count ++;
	
		DP_MSRP_WRITE2( "CMsrpPayloadFormatWrite::GenerateRedPacket DATA[4]: %x", iData[4] );
		DP_MSRP_WRITE2( "CMsrpPayloadFormatWrite::GenerateRedPacket DATA[5]: %x", iData[5] );
		DP_MSRP_WRITE2( "CMsrpPayloadFormatWrite::GenerateRedPacket DATA[6]: %x", iData[6] );
		DP_MSRP_WRITE2( "CMsrpPayloadFormatWrite::GenerateRedPacket DATA[7]: %x", iData[7] );
		DP_MSRP_WRITE2( "CMsrpPayloadFormatWrite::GenerateRedPacket DATA[8]: %x", iData[8] );
		DP_MSRP_WRITE( "*******************************");
		
		//TODO: When IOP-Tests Are done, Write me properly using Append, or some other method
		TInt forHere = count+r2.iData.Length();
		TInt index(0);
		if ( r2.iData.Length() )
		{
			for ( TInt i = count; i < forHere; i++ )
			  	{
				iData[i] = r2.iData[index];
				index++,
				RDebug::Print( _L("	X Writing R2 DATA[i]: %x"), iData[i] );
				}
			
		dataP += r2.iData.Length();
		count += r2.iData.Length();		
		}
			
			
		forHere = count+r1.iData.Length();
		index = 0;
		if ( r1.iData.Length() )
		{
			for ( TInt i = count; i < forHere; i++ )
			  	{
				iData[i] = r1.iData[index];
				index++;
				RDebug::Print( _L("	X Writing R1 DATA[i]: %x"), iData[i] );
				}
			
		dataP += r1.iData.Length();		
		count += r1.iData.Length();
		}
 
		
		forHere = count+primary.iData.Length();
		index = 0;
		if ( primary.iData.Length() )
		{
			for ( TInt i = count; i < forHere; i++ )
			  	{
				iData[i] = primary.iData[index];
				index++;
				RDebug::Print( _L("	X Writing PRIMARY DATA[i]: %x"), iData[i] );
				}
			
		dataP += primary.iData.Length();		
		count += primary.iData.Length();
		}
				
				
		DP_MSRP_WRITE2( "CMsrpPayloadFormatWrite::GenerateRedPacket Writing Lenght: %d", count );
		iBuf->Des().Append( iData, count );
		return KErrNone;
		
}


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::DeliverPacketL
// Prepare the packet header and deliver the packet to the datasink.
// -----------------------------------------------------------------------------
void CMsrpPayloadFormatWrite::DeliverPacketL( CMMFDataBuffer& aPayload )
    {
    
    DP_MSRP_WRITE("CMsrpPayloadFormatWrite::DeliverPacketL"); 
   
    if ( !iClip )
        {
        User::Leave( KErrNotReady ); 
        }
    const TMediaId mediaId( KUidMediaTypeAudio );
    const TInt dataSize( aPayload.Data().Size() );
    DP_MSRP_WRITE2( "MSRP Payload SIZE : %d", dataSize );
    
    if ( dataSize )
        {           
        if (iIsMsrpSink)
            {
            iMsrpDataSink->EmptyBufferL( &aPayload, this, mediaId );
            }
        }       
    aPayload.Data().Zero();
    }


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::SourceBufferEmptiedL
// Handle the event that sourcebuffer has been emptied.
// Sourcebuffer is given in "iSourceBuffer".
// Called by the statemachine.
// -----------------------------------------------------------------------------
void CMsrpPayloadFormatWrite::SourceBufferEmptiedL()
    { 
    
    
    DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::SourceBufferEmptiedL" ); 
    
    //Empty all
    iSourceBuffer->Data().FillZ();  
	iSourceBuffer->Data().Zero();
	iSinkBuffer->Data().FillZ();  
	iSinkBuffer->Data().Zero();
    iBuf->Des().FillZ();
    iBuf->Des().Zero();
    iDataPath->BufferEmptiedL( iSourceBuffer );

	//Finally we set the timer if we are in red mode 	
	switch( iCodecInfo.iRedundancyCount )
	{
		case 0:
				// do nothing 
		break;
		
		case 2:
			if ( iIdleMode )
				{
				iTimer->Cancel();
				iTimer->Start();	
				}
		break;
	}
	
	// we are no more in idle,
	iIdleMode = EFalse;
	
}


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::SinkPrimeL
// Prime the sink
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatWrite::SinkPrimeL()
    {
	DP_MSRP_WRITE("CMsrpPayloadFormatWrite::SinkPrimeL()");
	iClip->SinkPrimeL();
    }


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::SinkPrimeL
// Prime the sink
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatWrite::CancelUlRequest() 
    {
    
	DP_MSRP_WRITE("CMsrpPayloadFormatWrite::CancelUlRequest()" );
    iStateMachine->Cancel();
    iStateMachine->ChangeState( EEncodeIdle );

    // Reset the payload buffer
    if ( iSinkBuffer )
        {
        TDes8& dataDes = static_cast<CMMFDataBuffer*>( iSinkBuffer )->Data();
        dataDes.SetLength( 0 );
        }
        
    }
    
    
// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::SinkPlayL
// Start playing.
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatWrite::SinkPlayL()
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatWrite::SinkPlayL" );
    // Allocate buffer for data transfer between 
    // FormatWrite - MDataSink
    delete iSinkBuffer;
    iSinkBuffer = NULL;
    iSinkBuffer = CMMFDataBuffer::NewL( KRedHeaderSize + KDataSize );
    
    // Start state machine
    iStateMachine->ChangeState( EWaitForSourceData );
    
    iFirstPacketFinished = EFalse;

    // Start a new cycle of frame collecting
    if ( iSinkBuffer )
        {
        iSinkBuffer->SetLastBuffer( EFalse );
        }

    iClip->SinkPlayL();
    }


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::SinkPauseL
// Pause sink
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatWrite::SinkPauseL()
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatWrite::SinkPauseL" );
	this->CancelUlRequest();
    iClip->SinkPauseL();
    
    }

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::SinkStopL
// Stop sink
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatWrite::SinkStopL()
    {
    DP_MSRP_WRITE("CMsrpPayloadFormatWrite::SinkPauseL" ); 
    // Stop state machine
   	this->CancelUlRequest();
    iClip->SinkStopL(); 
    }


// ---------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::ConfigurePayloadFormatL
// Configure payload encoding parameters
// ---------------------------------------------------------------------------
//
void CMsrpPayloadFormatWrite::ConfigurePayloadFormatL( const TDesC8& aConfigParams, 
    CMccRtpMediaClock& /*aClock*/  )
    {
    DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::ConfigurePayloadFormatL()" );
   	
   //Check that buffer is same size, so we can assume data is correct type
    __ASSERT_ALWAYS( aConfigParams.Size() == sizeof( TMccCodecInfo ),
        User::Leave( KErrArgument ) );
     
    //instantiate correct CodecInfo Structure
    TMccCodecInfoBuffer infoBuffer;
    infoBuffer.Copy( aConfigParams );
    TMccCodecInfo codecInfo( infoBuffer() ); 
    
	if ( !codecInfo.iIsUpdate )
		{

		//iCodecInfo = codecInfo;
		//iRtpMediaClock = &aClock;  
		//iKey = iRtpMediaClock->RegisterMediaFormat( KDefaultSampleRate, iCodecInfo.iHwFrameTime );
		}
	else
		{
		UpdateConfigurationL( aConfigParams );
		} 

    }

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::UpdateConfigurationL
// Update payload encoding parameters
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatWrite::UpdateConfigurationL( const TDesC8& aConfigParams )
    { 
    DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::UpdateConfigurationL" );
    __ASSERT_ALWAYS( aConfigParams.Size() == sizeof( TMccCodecInfo ),
        User::Leave( KErrArgument ) );
    
    TMccCodecInfoBuffer infoBuffer;
    infoBuffer.Copy( aConfigParams );
    TMccCodecInfo codecInfo = infoBuffer();
    
    iCodecInfo = codecInfo;
    //iCodecInfo.iPtime = codecInfo.iPtime;
    //iCodecInfo.iRedundancyCount = 0;
    
    //iCodecInfo.iPayloadType = codecInfo.iPayloadType;
    //iCodecInfo.iRedundancyCount = codecInfo.iRedundancyCount;
    //iCodecInfo.iCodecMode = codecInfo.iCodecMode;
    
    //TODO: move me to right variable and update buffering time
    //iTimer->Cancel();
    //iTimer->SetTimeOut( iCodecInfo.iCodecMode * 1000);
    
    // Allocate buffer for data transfer between 
    // FormatWrite - MDataSink
    delete iSinkBuffer;
    iSinkBuffer = NULL;
    iSinkBuffer = CMMFDataBuffer::NewL( KRedHeaderSize + KDataSize );
    
    }



// ==========================    HELPER FUNCTIONS    =========================
// ========================== 						 =========================
// ========================== 						 =========================

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::Write24
// Simple helper function, that writes a 24-bit value to databuffer, 
// using pointer
// -----------------------------------------------------------------------------
//
void CMsrpPayloadFormatWrite::Write24( TUint8* const aPointer, TUint32 aValue )
    {
    DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::Write24" );
    aPointer[0] = static_cast<TUint8>( ( aValue & 0xFF0000 ) >> 16 );
    aPointer[1] = static_cast<TUint8>( ( aValue & 0x00FF00 ) >> 8 );
    aPointer[2] = static_cast<TUint8>( aValue & 0x0000FF );
    }
    

// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::Write32
// Simple helper function, that writes a 32-bit value to databuffer, 
// using pointer
// -----------------------------------------------------------------------------
//    
void CMsrpPayloadFormatWrite::Write32( TUint8* const aPointer, TUint32 aValue )
	{
    DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::Write32" );
	aPointer[0] = static_cast<TUint8>( ( aValue & 0xFF000000 ) >> 24 );
	aPointer[1] = static_cast<TUint8>( ( aValue & 0x00FF0000 ) >> 16 );
	aPointer[2] = static_cast<TUint8>( ( aValue & 0x0000FF00 ) >> 8 );
	aPointer[3] = static_cast<TUint8>( aValue & 0x000000FF );
	}


// -----------------------------------------------------------------------------
// CMsrpPayloadFormatWrite::SetTimeOffset
// Calculates TimeOffset to RedundancyHeader according to 
// RFC 4103. Actually RFC 4103 does not accurately state how ( and where )
// timeoffset should be calculated. This calculation is based conversations
// with authors of RFC 4103
// -----------------------------------------------------------------------------
//
TUint16 CMsrpPayloadFormatWrite::SetTimeOffset( TInt64& aPrim, TInt64& aSecond  )
{	
    DP_MSRP_WRITE( "CMsrpPayloadFormatWrite::SetTimeOffset" );
	TInt pOffset = aPrim - aSecond; 
	TUint16 aRet = TUint16(pOffset) & KHighestTimeStampOffset;
	if( aRet > KHighestTimeStampOffset )
	{
		aRet = KHighestTimeStampOffset;
	}
	return aRet; 
}


TStringElement::TStringElement()
{
}


//  End of File
