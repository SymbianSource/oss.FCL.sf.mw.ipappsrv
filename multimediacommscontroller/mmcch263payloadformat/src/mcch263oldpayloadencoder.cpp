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

#include "mcch263oldpayloadencoder.h"
#include "mcch263formatlogs.h"

// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------
// CIPMediaVideoSink::NewL()
// First stage constructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CMccH263OldPayloadEncoder* CMccH263OldPayloadEncoder::NewL()
    {
    return new ( ELeave ) CMccH263OldPayloadEncoder();
    }

CMccH263OldPayloadEncoder::~CMccH263OldPayloadEncoder()
	{
	}

// -----------------------------------------------------------------------------
// CIPMediaVideoSink::WriteBufferL()
// Write media buffer
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMccH263OldPayloadEncoder::EncodeL( 
	CMMFBuffer* aMMFBuffer,
    RPointerArray<CBufferContainer>& aDesBuffers )
    {
    __H263FORMAT_MEDIA("CMccH263OldPayloadEncoder::EncodeL, enter")

    CMMFDataBuffer* buf = static_cast<CMMFDataBuffer*>( aMMFBuffer );
 	iSendHeader.iMarker = 0;

	// Get timestamp to RTP header. conversion to 90kHz clock
	TInt64 tmpStamp = ( buf->TimeToPlay().Int64() + iTimeStamp.Int64() ) /  KConst100 * KConst9 ;
	iSendHeader.iTimestamp = TRtpTimeStamp( tmpStamp ) ;
  
    __H263FORMAT_MEDIA_INT2( "CMccH263OldPayloadEncoder::EncodeL, TimeStamp:", 
    iSendHeader.iTimestamp, "Length:", buf->BufferSize() )
    
    // Reset marker from last frame
	iSendHeader.iMarker = 0;
	
	// Get pointer to raw data
	TPtrC8 frame = buf->Data();
	
    // Check that descriptor length is included. 
    // Missing length causes panic in frame.Mid() function
    if ( frame.Length() == 0 || buf->BufferSize() != frame.Length() )
        {
        __H263FORMAT_MEDIA("CMccH263OldPayloadEncoder::EncodeL, ERROR Buffer corrupted")
        User::Leave( KErrBadDescriptor );
        }
        
	const TUint8 *dataPtr = buf->Data().Ptr();
	// Get buffer length
	TUint frameLength = buf->BufferSize();
    
    // Get frame header information
	GetHeaderInfoL( frame );
	
    // If packet is smaller than max Packet size. Send right away
    if ( frame.Length() <= TInt( KVideoPacketSize ) )
        {
        __H263FORMAT_MEDIA("CMccH263OldPayloadEncoder::EncodeL, Sending whole frame...")

		// Set marker to indicate last packet of current frame
        CreateNewBufferL( aMMFBuffer,  
                          aDesBuffers, 
                          frame, 
                          frameLength + KConst4, 
                          1 );
		
        __H263FORMAT_MEDIA("CMccH263OldPayloadEncoder::EncodeL, exit" )
        return;
        }
    
	// Initialize splitting variables
	TInt lastGOB = 0;
	TInt lastsend = 0;
	
	// Scan frame for GOBs
	for( TUint i= 0; i < frameLength; i++ )
	    {   
		dataPtr += i;
		if ( *dataPtr == TUint8( 0x00 ) )
		    {
			dataPtr++;
			if ( *dataPtr == TUint8( 0x00 ) )
			    { 
				dataPtr++;
				if ( TUint8( *dataPtr&0xfc )>0x80 )
				    {
					if ( ( i - lastsend ) >= KVideoPacketSize && lastGOB )
					    {
                       	__H263FORMAT_MEDIA_INT2("CMccH263OldPayloadEncoder::EncodeL(scan gobs), Sending packet f",
                       	lastsend, "len", ( lastGOB - lastsend ) )

						// Packet size exceeded send new RTP Packet
						TPtrC8 packetData = 
							frame.Mid( lastsend, ( lastGOB - lastsend ) );
							
						CreateNewBufferL( aMMFBuffer,  
                                          aDesBuffers, 
                                          packetData, 
                                          packetData.Length() + KConst4, 
                                          iSendHeader.iMarker );

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
    	__H263FORMAT_MEDIA_INT2("CMccH263OldPayloadEncoder::EncodeL(last packet check), Sending packet f", 
    	lastsend, "len", ( lastGOB - lastsend ) )

		TPtrC8 packetData = 
			frame.Mid( lastsend, ( lastGOB - lastsend ) );
			
	    CreateNewBufferL( aMMFBuffer,  
                          aDesBuffers, 
                          packetData, 
                          packetData.Length() + KConst4, 
                          iSendHeader.iMarker );
		
		lastsend = lastGOB;
	    }
	    
	// Send last packet to RTP API
	if ( lastsend != 0 )
	    {
	    __H263FORMAT_MEDIA_INT2("CMccH263OldPayloadEncoder::EncodeL(last packet), Sending packet f", 
	    lastsend, "len", ( frameLength - lastsend ) )
	
		TPtrC8 packetData = 
			frame.Mid( lastsend, ( frameLength - lastsend ) );

		// Set marker to indicate last packet of current frame
	    CreateNewBufferL( aMMFBuffer,  
                          aDesBuffers, 
                          packetData, 
                          packetData.Length() + KConst4, 
                          1 );
		
		}
		
	// Special case no GOBs found try to send whole frame
	else
	    {       
        __H263FORMAT_MEDIA_INT1("CMccH263OldPayloadEncoder::EncodeL(no gobs), Sending whole packet len", 
        frameLength )
	    
		if ( buf->BufferSize() > TInt( KVideoPacketSize + KConst4 ) )
            {
            __H263FORMAT_MEDIA("CMccH263OldPayloadEncoder::EncodeL, ERROR, too big frame, exit")

            return;
            }

		// Set marker to indicate last packet of current frame
        CreateNewBufferL( aMMFBuffer,  
                          aDesBuffers, 
                          frame, 
                          frameLength + KConst4, 
                          1 );
		
		}

    __H263FORMAT_MEDIA("CMccH263OldPayloadEncoder::EncodeL, exit")
 
    }

// -----------------------------------------------------------------------------
// CMccH263OldPayloadEncoder::WriteHeaderData()
// Write payload header information byte
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUint8 CMccH263OldPayloadEncoder::WriteHeaderData( TInt aByte )
    {
    __H263FORMAT_MEDIA("CMccH263OldPayloadEncoder::WriteHeaderData")

    //byte to be written
    TUint8 wbyte = 0;
	//construct needed byte
    switch ( aByte )
	    {
	    case 0:
		    {
			wbyte |= TUint8( headerA.F << KConst7 );
			wbyte |= TUint8( headerA.P << KConst6 );
			wbyte |= TUint8( headerA.SBIT << KConst3 );
			wbyte |= TUint8( headerA.EBIT );
		    }break;
	    case 1:
		    {
			wbyte |= TUint8( headerA.SRC << KConst5 );
			wbyte |= TUint8( headerA.I << KConst4 );
			wbyte |= TUint8( headerA.U << KConst3 );
			wbyte |= TUint8( headerA.S << KConst2 );
			wbyte |= TUint8( headerA.A << 1 );
		    }break;
	    case KConst2:
		    {
			wbyte |= TUint8( headerA.DBQ << KConst3 );
			wbyte |= TUint8( headerA.TRB );
		    }break;
	    case KConst3:
		    {
			wbyte = headerA.TR;
		    }break;
	    default:
		    {
		    }break;
	    }
	//return constructed byte
    return wbyte;
    }

// -----------------------------------------------------------------------------
// CMccH263OldPayloadEncoder::GetHeaderInfoL()
// Get video frame header information
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMccH263OldPayloadEncoder::GetHeaderInfoL( TDesC8& aFrame )
    {
    __H263FORMAT_MEDIA("CMccH263OldPayloadEncoder::GetHeaderInfoL")

	//fill in default values = 0
	headerA.F = 0;
	headerA.P = 0;
	headerA.SBIT = 0;
	headerA.EBIT = 0;
	headerA.SRC = 0;
	headerA.I = 0;
	headerA.U = 0;
	headerA.S = 0;
	headerA.A = 0;
	headerA.R = 0;
	headerA.DBQ = 0;
	headerA.TRB = 0;
	headerA.TR = 0;

	//A S S U M I N G that aFrame begins with PSC
	const TUint8 *help;
	help = aFrame.Ptr();
	//goto PTYPE-field
	help += KConst4;
	TUint8 test = 0;
	//get SRC
	test = TUint8( *help & 0x1c );
	test >>= KConst2;
	headerA.SRC = test;
	if( headerA.SRC == KConst7 )
	    {
        #ifdef LOG
	        IPUL_LOGC( 1, "VSink::GetHeaderInfoL> ERROR: Picture format unsupported" );
        #endif
	    }
	//get I parameter
	test = 0;
	test = TUint8( *help & 0x02 );
	test >>= 1;
	headerA.I = test;
	//get U parameter
	test = 0;
	test = TUint8( *help & 0x01 );
	headerA.U = test;
	//get S parameter
	help++;
	test = 0;
	test = TUint8( *help & 0x80 );
	test >>= KConst7;
	headerA.S = test;
	//get A parameter
	test = 0;
	test = TUint8( *help & 0x40 );
	test >>= KConst6;
	headerA.A = test;
	//get P parameter
	test = 0;
	test = TUint8( *help & 0x20 );
	test >>= KConst5;
	headerA.P = test;
	//check for correct file format
	if ( headerA.P != 0 )
	    {
        #ifdef LOG
	    IPUL_LOGC( 1 , "VSink::GetHeaderInfoL> ERROR: Unsupported file format" );
        #endif
	    }
    }
    
// -----------------------------------------------------------------------------
// CMccH263OldPayloadEncoder::SetPayloadType()
// -----------------------------------------------------------------------------
//
void CMccH263OldPayloadEncoder::SetPayloadType( TUint8 aPayloadType )
	{
	__H263FORMAT_MEDIA("CMccH263OldPayloadEncoder::SetPayloadType")
	
	iSendHeader.iPayloadType = aPayloadType;
	}

// -----------------------------------------------------------------------------
// CMccH263OldPayloadEncoder::CreateNewBufferL()
// -----------------------------------------------------------------------------
//
void CMccH263OldPayloadEncoder::CreateNewBufferL( 
	CMMFBuffer* aMMFBuffer,  
	RPointerArray<CBufferContainer>& aDesBuffers,
	const TDesC8& aPacketData,
	TInt aLength,
	TInt aMarker )
	{
	CMMFDataBuffer* newBuffer = 
		CMMFDataBuffer::NewL( aLength );                          
	CleanupStack::PushL( newBuffer );
	newBuffer->Data().Append( WriteHeaderData( 0 ) );
	newBuffer->Data().Append( WriteHeaderData( 1 ) );
	newBuffer->Data().Append( WriteHeaderData( KConst2 ) );
	newBuffer->Data().Append( WriteHeaderData( KConst3 ) );
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

