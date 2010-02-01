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



// ============================ INCLUDES =======================================

#include "mcch263newpayloadencoder.h"
#include "mcch263formatlogs.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccH263NewPayloadEncoder::NewL()
// First stage constructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CMccH263NewPayloadEncoder* CMccH263NewPayloadEncoder::NewL()
    {
    return new ( ELeave ) CMccH263NewPayloadEncoder();
    }
    
CMccH263NewPayloadEncoder::~CMccH263NewPayloadEncoder()
	{	
	}

// -----------------------------------------------------------------------------
// CMccH263NewPayloadEncoder::WriteBufferL()
// Write media buffer
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMccH263NewPayloadEncoder::EncodeL( 
	CMMFBuffer* aMMFBuffer,  
	RPointerArray<CBufferContainer>& aDesBuffers  )
	{
	__H263FORMAT_MEDIA("CMccH263NewPayloadEncoder::EncodeL, enter")
	
	const TInt KMccValZero = 0;
	const TInt KMccValOne = 1;

    CMMFDataBuffer* buf = static_cast<CMMFDataBuffer*>( aMMFBuffer );
 	iSendHeader.iMarker = 0;

    // Get timestamp to RTP header. conversion to 90kHz clock
	TInt64 tmpStamp = 
	    ( buf->TimeToPlay().Int64() + iTimeStamp.Int64() ) / KConst100 * KConst9 ;
	iSendHeader.iTimestamp = TRtpTimeStamp( tmpStamp ) ;

    __H263FORMAT_MEDIA_INT2("CMccH263NewPayloadEncoder::EncodeL > TimeStamp:",
    iSendHeader.iTimestamp, "Length:", buf->BufferSize())
   
    // Reset marker from last frame
	iSendHeader.iMarker = 0;
	// Get pointer to raw data
	TPtrC8 frame = buf->Data();
    
    // Check that descriptor length is included. 
    // Missing length causes panic in frame.Mid() function
    if ( frame.Length() == 0 || buf->BufferSize() != frame.Length() )
        {
        __H263FORMAT_MEDIA ("CMccH263NewPayloadEncoder::EncodeL, ERROR Buffer corrupted")
        User::Leave( KErrBadDescriptor );
        }
	const TUint8 *dataPtr = buf->Data().Ptr();
	
	// Get buffer length
	TUint frameLength = buf->BufferSize();
	
    // Initialize splitting variables
	TInt lastGOB = 0;
	TInt lastsend = 0;
	TPtrC8 packetData;
	
	// Scan frame for GOBs
	for( TUint i= 0; i < frameLength - KConst2; i++ )
	    {   
		dataPtr += i;
		if ( *dataPtr == TUint8( 0x00 ) )
		    {
			dataPtr++;
			if ( *dataPtr == TUint8( 0x00 ) )
			    { 
				dataPtr++;
				if ( TUint8( *dataPtr&0xfc ) > 0x80 )
				    {	
				    if ( ( i - lastsend ) >= KVideoPacketSize && 
				         ( i - lastsend ) <= KMaxPacketSize &&
				         lastGOB )
					    {
                        __H263FORMAT_MEDIA_INT2(
                        "CMccH263NewPayloadEncoder::EncodeL(scan gobs), Sending packet f:",
                        lastsend, "len:", ( lastGOB - lastsend ) )

						// Packet size exceeded send new RTP Packet
				        // +2 exclude first 2 0-bytes
						packetData.Set( 
						    frame.Mid( ( lastsend + KConst2 ), 
						               ( lastGOB - lastsend ) - KConst2 ) );
						
                        CreateNewBufferL( aMMFBuffer,  
                                          aDesBuffers, 
                                          packetData, 
                                          packetData.Length() + KConst4, 
                                          KMccValZero,
                                          KMccValZero,
                                          KMccValOne );
						
						lastsend = lastGOB;
					    }
					    
					lastGOB = i;
				    }
			    }
		    } 
		// Return pointer to begining of frame
		dataPtr = frame.Ptr();
	    }
	    
    // Check that last packet does not exceed packet size
	if ( ( frameLength - lastsend ) >= KVideoPacketSize && lastsend != 0 )
	    {
        __H263FORMAT_MEDIA_INT2(
        "CMccH263NewPayloadEncoder::EncodeL(last packet check), Sending packet f:",
        lastsend, "len:", ( lastGOB - lastsend ) )

         // +2 exlude first 2 0-bytes
		packetData.Set(
			frame.Mid( ( lastsend + KConst2 ), ( lastGOB - lastsend ) - KConst2 ) );
 		
 		CreateNewBufferL( aMMFBuffer,  
 		                  aDesBuffers, 
 		                  packetData, 
 		                  packetData.Length() + KConst4, 
 		                  KMccValZero,
 		                  KMccValZero,
 		                  KMccValOne );
		
		lastsend = lastGOB;
	    }
	    
	// Send last packet to RTP API
	if ( lastsend != 0 )
	    {
	    __H263FORMAT_MEDIA_INT2(
	    "CMccH263NewPayloadEncoder::EncodeL(last packet), Sending packet f:", 
	    lastsend, "len:", ( frameLength - lastsend ) )

	    // +2 exlude first 2 0-bytes
		packetData.Set(
			frame.Mid( ( lastsend + KConst2 ),( frameLength - lastsend ) - KConst2 ) ); 
		
		// Set marker to indicate last packet of current frame
        CreateNewBufferL( aMMFBuffer,  
                          aDesBuffers, 
                          packetData, 
                          packetData.Length() + KConst4, 
                          KMccValOne,
                          KMccValZero,
                          KMccValOne );
		
		}
		
	// Special case no GOBs found try to send whole frame
	else
	    {
		if ( buf->BufferSize() > TInt( KVideoPacketSize + KConst2 ) )
            {
            __H263FORMAT_MEDIA(
            "CMccH263NewPayloadEncoder::EncodeL(no gobs), divide to multiple packets")
            
            // First packet            
            // 2 exlude first 2 0-bytes
            packetData.Set( frame.Mid( KConst2, ( KVideoPacketSize - KConst2 ) ) );
            
            // Set marker to indicate last packet of current frame
            CreateNewBufferL( aMMFBuffer,  
                              aDesBuffers, 
                              packetData, 
                              KVideoPacketSize, 
                              KMccValZero,
                              KMccValZero,
                              KMccValOne );

	        // Other pieces
            TUint index = KVideoPacketSize, size;
    	    while (index < buf->BufferSize())
	            {
	            __H263FORMAT_MEDIA("CMccH263NewPayloadEncoder::EncodeL(no gobs), looping")
	            
	            size= buf->BufferSize() - index;
                packetData.Set(
                	frame.Mid( index, Min( TInt( KVideoPacketSize - KConst2 ), size ) ) );

    	        // Set marker to indicate last packet of current frame
    	        TInt marker = size > TInt( KVideoPacketSize ) ? KMccValZero : KMccValOne;

	            CreateNewBufferL( aMMFBuffer,  
	                              aDesBuffers, 
	                              packetData, 
	                              Min( TInt( KVideoPacketSize ), size + KConst2 ), 
	                              marker,
	                              KMccValOne,
	                              KMccValOne );

                index += KVideoPacketSize - KConst2;
	            }
            }
         else 
            {
            __H263FORMAT_MEDIA("CMccH263NewPayloadEncoder::EncodeL(no gobs), fits one packet")
            
            // 2 exlude first 2 0-bytes
            packetData.Set( frame.Mid( KConst2, ( frameLength - KConst2 ) ) );

			CreateNewBufferL( aMMFBuffer, 
			                  aDesBuffers, 
			                  packetData,frameLength, 
			                  KMccValOne,
			                  KMccValZero,
			                  KMccValOne );

            }
	    }

	__H263FORMAT_MEDIA("CMccH263NewPayloadEncoder::EncodeL, exit")
  
    }
// -----------------------------------------------------------------------------
// CMccH263NewPayloadEncoder::WriteHeaderData()
// Write payload header information byte
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUint8 CMccH263NewPayloadEncoder::WriteHeaderData( TInt aByte )
    {
    __H263FORMAT_MEDIA("CMccH263NewPayloadEncoder::WriteHeaderData")

    TUint8 wbyte = 0;
    switch ( aByte )
    {
    case 0:
        {
        wbyte |= TUint8( 1 << KConst2 ); //indicate that fist 2 0-bytes are excluded
        }break;                    //from start codes
    case 1:
        {
        }break;
    default:
        {
        }break;
    }
    return wbyte;
    }
// -----------------------------------------------------------------------------
// CMccH263NewPayloadEncoder::SetPayloadType()
// -----------------------------------------------------------------------------
//
void CMccH263NewPayloadEncoder::SetPayloadType( TUint8 aPayloadType )
	{
	__H263FORMAT_MEDIA("CMccH263NewPayloadEncoder::SetPayloadType")
	iSendHeader.iPayloadType = aPayloadType;
	}

// -----------------------------------------------------------------------------
// CMccH263NewPayloadEncoder::CreateNewBufferL()
// -----------------------------------------------------------------------------
//
void CMccH263NewPayloadEncoder::CreateNewBufferL( 
	CMMFBuffer* aMMFBuffer,  
	RPointerArray<CBufferContainer>& aDesBuffers,
	const TDesC8& aPacketData,
	TInt aLength,
	TInt aMarker,
	TInt aHeaderDataByteOne,
	TInt aHeaderDataByteTwo )
	{
	CMMFDataBuffer* newBuffer = 
		CMMFDataBuffer::NewL( aLength );                          
	CleanupStack::PushL( newBuffer );
	newBuffer->Data().Append( WriteHeaderData( aHeaderDataByteOne ) );
	newBuffer->Data().Append( WriteHeaderData( aHeaderDataByteTwo ) );
	newBuffer->Data().Append( aPacketData );
	newBuffer->SetTimeToPlay( aMMFBuffer->TimeToPlay() );
	newBuffer->SetFrameNumber( aMMFBuffer->FrameNumber() + iSequenceNumIncrementer );
	
	// If frame is divided in several packets, sequence numbers of following
	// packets have to be modified
	if ( aMarker == 0 )
	    {
	    iSequenceNumIncrementer++;
	    }

	CBufferContainer* cont = new (ELeave) CBufferContainer();
	CleanupStack::Pop( newBuffer );
	cont->iBuffer = newBuffer;
	iSendHeader.iMarker = aMarker;
	cont->iSendHeader = iSendHeader;
	CleanupStack::PushL( cont );	    
	aDesBuffers.AppendL( cont );
	CleanupStack::Pop( cont );
    }

