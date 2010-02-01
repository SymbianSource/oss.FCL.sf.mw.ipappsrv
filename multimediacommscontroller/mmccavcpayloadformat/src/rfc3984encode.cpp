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
* Description:    AVC Payloadization class
*
*/




// ============================ INCLUDES =======================================

#include"rfc3984encode.h"
#include "mccinternalcodecs.h"
#include <es_sock.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRFC3984Encode::CRFC3984Encode()
// Default Constructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRFC3984Encode::CRFC3984Encode()
    {
	iBufferIndex = 0;
	iToPayloadizeCount = 0;
	iNalCount = 0;
    }

// -----------------------------------------------------------------------------
// CRFC3984Encode::~CRFC3984Encode()
// Default Destructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRFC3984Encode::~CRFC3984Encode()
    {
	ClearNalBuffers();
	iToPayloadizeBuffer.Reset( );
	iToPayloadizeSizeBuffer.Reset( );	
    }

// -----------------------------------------------------------------------------
// CRFC3984Encode::NewL()
// First stage constructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

CRFC3984Encode * CRFC3984Encode::NewL()
    {
	CRFC3984Encode * self = new(ELeave) CRFC3984Encode;
	CleanupStack::PushL( self );
    CleanupStack::Pop();
    return self;		
    }


// -----------------------------------------------------------------------------
// CRFC3984Encode::SetMTUSize()
// Purpose     : 
// Parameters  : 
// Return Value: 
// -----------------------------------------------------------------------------
//

void CRFC3984Encode::SetMTUSize( TUint32 aMtuSize )
    {
	iMaxPacketSize = aMtuSize;	
    }


// -----------------------------------------------------------------------------
// CRFC3984Encode::SetFrameRate()
// Purpose     : 
// Parameters  : 
// -----------------------------------------------------------------------------
//

void CRFC3984Encode::SetFrameRate( TInt aFrameRate )
    {
	iFrameRate = aFrameRate;	
    }

// -----------------------------------------------------------------------------
// CRFC3984Encode::PayloadizeFrameL()
// Purpose     : This function receives the encoded frame and payloadizes it based 
//               on the packetization_mode value
// Parameters  : aBuffer - Buffer containing the encoded frame
//				 aTimeStamp - Timestamp to go into RTP
// 				 aMarkerBit - Marker Bit indication (this function sets it for key frames)
// 				 aNalCount - Reference to return the number of NAL units payloadized
// Return Value: none, leaves if an error occured
// -----------------------------------------------------------------------------
//

void CRFC3984Encode::PayloadizeFrameL(
    TDes8 & aBuffer, 
    TUint32 aTimeStamp, 
    TUint32 & aMarkerBit, 
    TInt &aNalCount )
    {
	TInt error = KErrNone;
	
	if ( iPacketizationMode == AVC_MODE_SNALU )
	    {
		TRAP( error, 
		PayloadizeFrameSingleNALModeL( aBuffer, aTimeStamp, aMarkerBit, aNalCount ) );
	    }
	else if ( iPacketizationMode == AVC_MODE_NONINTERLEAVED )
	    {
		TRAP( error, 
		PayloadizeFrameNonInterleavedModeL( aBuffer, aTimeStamp, aMarkerBit, aNalCount ) );
    	}
	else if ( iPacketizationMode == AVC_MODE_INTERLEAVED )
	    {
		User::Leave( KErrNotSupported );
	    }
	
	if ( KErrNone != error )
	    {
		ClearNalBuffers();
		iToPayloadizeBuffer.Reset();
    	iToPayloadizeSizeBuffer.Reset();
    	iToPayloadizeCount = 0;
    	User::Leave( error );
        }
    }

// -----------------------------------------------------------------------------
// CRFC3984Encode::PayloadizeFrameSingleNALModeL()
// Purpose     : This function receives the encoded frame and payloadizes it
//               according to the SNALU mode
// Parameters  : aBuffer - Buffer containing the encoded frame
//				 aTimeStamp - Timestamp to go into RTP
// 				 aMarkerBit - Marker Bit indication (this function sets it for key frames)
//				 aNalCount - This function returns the number of packetized NAL
//               units through this variable.
// Return Value: none, leaves if an error occured
// -----------------------------------------------------------------------------
//

void CRFC3984Encode::PayloadizeFrameSingleNALModeL(
    TDes8& aBuffer, 
    TUint32 /*aTimeStamp*/, 
    TUint32& /*aMarkerBit*/, 
    TInt& aNalCount )
    {
	// This implementation assumes the NAL units for one frame are seperated
	// by 0x001 or 0x0001 start codes 
    
    // Resetting iBufferIndex for this new frame, always do this for new frame
    // because this is used to find start codes within one frame
    iBufferIndex = 0; 	
    
    TInt startIndex = 0;	// holds first byte-index of NALU
    TInt endIndex = 0;		// hold byte-index of next to last byte of NALU
    TInt size = 0;
    HBufC8 * pBuffer = 0;
    
    // loop to find and packetize all NAL Units in the frame
	while ( ETrue )
	    {
		startIndex = TMccCodecInfo::FindAvcNaluStart( iBufferIndex, aBuffer );
		if ( KErrNotFound == startIndex )
		    {
			break;
		    }
		
		endIndex = TMccCodecInfo::FindAvcNaluEnd( iBufferIndex, aBuffer );
		if ( KErrNotFound == endIndex )
		    {
			break;
		    }
		
		if ( startIndex == endIndex )
		    {
			break;
		    }
				
		// finding size of the NAL unit
		size = endIndex - startIndex;
		
		__ASSERT_ALWAYS( size > 0, User::Leave( KErrGeneral ) ); // some flaw in logic
		
		pBuffer = HBufC8::NewLC( size );
		
		TPtr8 aPtr = pBuffer->Des();
		// Now the size and start Index is known, copying the data
		
		aPtr.Copy( aBuffer.Mid( startIndex, size ) );
		
		// Now inserting pointer
		iPayloadizedBuffers.InsertL( pBuffer, iNalCount );
		CleanupStack::Pop( pBuffer );
		pBuffer = NULL;	// ownership transferred
		iNalCount++;
	    }
    
    aNalCount = iNalCount;
    }

// -----------------------------------------------------------------------------
// CRFC3984Encode::PayloadizeFrameNonInterleavedModeL()
// Purpose     : This function receives the encoded frame and payloadizes it
//               according to the Non-Interleaved Mode
// Parameters  : aBuffer - Buffer containing the encoded frame
//				 aTimeStamp - Timestamp to go into RTP
// 				 aMarkerBit - Marker Bit indication (this function sets it for key frames)
//				 aNalCount - This function returns the number of packetized
//               NAL units through this variable.
// Return Value: none, leaves if an error occured
// -----------------------------------------------------------------------------
//

void CRFC3984Encode::PayloadizeFrameNonInterleavedModeL(
    TDes8& aBuffer, 
    TUint32 aTimeStamp, 
    TUint32& aMarkerBit, 
    TInt& aNalCount )
    {
	iNalCount = 0;
	aNalCount = 0; // resetting value coming from above
	iBufferIndex = 0;
	
	TInt startIndex = 0;
	TInt endIndex = 0;
	TInt size = 0;
	TInt count = 0; // variable to keep track of the size of aggregated packet.
	
	aNalCount = 0;
	
	// loop to find and packetize all NAL Units in the frame
	while( ETrue )
	    {
		startIndex = TMccCodecInfo::FindAvcNaluStart( iBufferIndex, aBuffer );
		if ( startIndex == KErrNotFound )
		    {
			break;
		    }
		
		endIndex = TMccCodecInfo::FindAvcNaluEnd( iBufferIndex, aBuffer );
		if ( endIndex == KErrNotFound )
		    {
			break;
		    }
		
		if ( startIndex == endIndex )
		    {
			break;
		    }
		    
		// finding size of the NAL unit
		size = endIndex - startIndex;
		
		__ASSERT_ALWAYS( size > 0, User::Leave( KErrGeneral ) ); // some flaw in logic
		
		// Now startIndex and size of this NAL Unit are known
		// decide if SNALU packetization or STAP or FU-A is required
		if ( size > iMaxPacketSize )
		    {
		    // if any NALU there for aggregation or SNALU, payloadize it first
			if ( iToPayloadizeCount > 0 )  
			    {
				PayloadizeNaluL( aBuffer, aTimeStamp, aMarkerBit, aNalCount );
				
				count = 0; // resetting count of bytes for aggregation packets
				iToPayloadizeCount = 0;	
			    }
			
			FragmentNaluL( aBuffer, aTimeStamp, aMarkerBit, aNalCount, startIndex, size, 0 );
		    }
		else if ( size <= iMaxPacketSize )
		    {
			if( size + count > iMaxPacketSize )
			    {
				// make STAP-A or SNALU based on number of elements in the buffer
				PayloadizeNaluL( aBuffer, aTimeStamp, aMarkerBit, aNalCount );
				
				count = 0;   // resetting count of bytes for aggregation packets
				iToPayloadizeCount = 0;
			    }	
			// Insert current NALU into the Buffer.
			iToPayloadizeBuffer.InsertL( startIndex, iToPayloadizeCount );
			iToPayloadizeSizeBuffer.InsertL( size, iToPayloadizeCount );
			iToPayloadizeCount++;
			count += size;
		    }
	    } // end while()
	
	// packetizing any last NALUs left in the buffer
	if ( iToPayloadizeCount > 0 )
	    {
	 	PayloadizeNaluL( aBuffer, aTimeStamp, aMarkerBit, aNalCount );
	 	count = 0;
	 	iToPayloadizeCount = 0;
	    }
    }


// -----------------------------------------------------------------------------
// CRFC3984Encode::PayloadizeNaluL()
// Purpose     : This function makes a SNALU packet or STAP-A packet based on the
//               number of NALUs in the array iToPayloadizeBuffer.
// Parameters  : aBuffer - Buffer containing the encoded frame
//				 aTimeStamp - Timestamp to go into RTP
// 				 aMarkerBit - Marker Bit indication (this function sets it for key frames)
//				 aNalCount - This function returns the number of packetized
//               NAL units through this variable.
//				 aStartIndex - Index to the starting of the NALU
// Return Value: none, leaves if an error occured
// -----------------------------------------------------------------------------
//

void CRFC3984Encode::PayloadizeNaluL(
    TDes8 & aBuffer, 
    TUint32 /*aTimeStamp*/, 
    TUint32& /*aMarkerBit*/, 
    TInt & aNalCount )
    {
	TInt startIndex = 0;
	TInt size = 0;
	
	__ASSERT_ALWAYS( iToPayloadizeCount > 0, User::Leave( KErrArgument ) );
	
	__ASSERT_ALWAYS( iToPayloadizeBuffer.Count() == iToPayloadizeSizeBuffer.Count(), 
	                 User::Leave( KErrArgument ) );
		
	if ( iToPayloadizeCount == 1 ) // SNALU packet
	    {
		startIndex = iToPayloadizeBuffer[0];
		size = iToPayloadizeSizeBuffer[0];	
		TPtr8 start = aBuffer.MidTPtr( startIndex );
		AddSnaluPacketL( start, size );
    	}
	else // payloadize STAP-A packet
	    {
		TInt count = 0;
		TInt totalSize = 0;
		TUint8 headerByte = 0;
		
		for ( count = 0; count < iToPayloadizeCount; count++ )
		    {
		    // finding total size of all the NALU's to aggregate
			totalSize += iToPayloadizeSizeBuffer[count]; 
		    }
		
		// addding the total size fields and STAP-A header size	
		totalSize += ( iToPayloadizeCount*2 + 1 ); 
		
		count = 0;
		
		// allocating memory for the total buffer size		
		HBufC8* pBuffer = HBufC8::NewLC( totalSize );
		
		TUint16 value = 0;
		TPtr8 pDes1 = pBuffer->Des();
		
		headerByte = PACKET_STAP_A | ( aBuffer[iToPayloadizeBuffer[count]] & ( 0x7 << 5 ) ); 
		pDes1.Append( &headerByte, 1 );
		
		 // Pps and sps are handled as SNALUs, 
		 // if there's nothing else to packetize, stap-a packet is not created
		TBool stapPacketCreated( EFalse );
		for( count = 0; count < iToPayloadizeCount; count++ )
		    {
			startIndex = iToPayloadizeBuffer[count];
			size = iToPayloadizeSizeBuffer[count];
			TPtr8 pStart = aBuffer.MidTPtr( startIndex ); // getting start index
			
			if ( TMccCodecInfo::IsAvcPpsOrSpsData( pStart, ETrue ) )
			    {
			    AddSnaluPacketL( pStart, size );
			    }
			else
			    {
    			// convert to network byte order
    			value = ByteOrder::Swap16( static_cast<TUint16>( size ) ); 
    			TUint8* ptrByte = reinterpret_cast<TUint8*>( &value );
    			pDes1.Append( ptrByte, 2 );
    			pDes1.Append( pStart.Ptr(), size );
    			stapPacketCreated = ETrue;
			    }
		    }
		
		if ( stapPacketCreated )
		    {
    		// inserting stap-a packet into the payloadized NAL unit buffer for retrieval
        	iPayloadizedBuffers.InsertL( pBuffer, iNalCount );
        	iNalCount++;
        	CleanupStack::Pop(pBuffer);
		    }
		else
		    {
		    // stap-a packet not created
		    CleanupStack::PopAndDestroy(pBuffer);
		    }
	    }
	
    //Now cleaning up the buffer
    iToPayloadizeBuffer.Reset();
    iToPayloadizeSizeBuffer.Reset();
    iToPayloadizeCount = 0;
    	
	aNalCount = iNalCount;
    }

// -----------------------------------------------------------------------------
// CRFC3984Encode::FragmentNaluL()
// Purpose     : This function receives the starting index of a NALU in a buffer and
//               fragments the NAL unit
// Parameters  : aBuffer - Buffer containing the encoded frame
//				 aTimeStamp - Timestamp to go into RTP
// 				 aMarkerBit - Marker Bit indication (this function sets it for key frames)
//				 aNalCount - This function returns the number of packetized NAL units
//               through this variable.
//				 aStartIndex - Index to the starting of the NALU
// 				 aSize - Size of the NALU
//				 aDon - valeu of Decoding order number , 16 bit value
// Return Value: none, leaves if an error occured
// -----------------------------------------------------------------------------
//

void CRFC3984Encode::FragmentNaluL(
    TDes8 & aBuffer, 
    TUint32 /*aTimeStamp*/, 
    TUint32 & aMarkerBit, 
    TInt & aNalCount, 
    TInt aStartIndex, 
    TInt aSize, 
    TUint16 aDON )
    {
	TInt index = 0;
	TInt fragsPacketized = 0;
	TUint8 headerByte = 0; // FU-A packet header byte (contains F, NRI, Type Fields) 
	TUint8 fragHeaderByte = 0;
	TInt length = 0;
	// maximum size of fragment, 4 is to cater for DON field in FU-B
	TInt fragMaxSize = iMaxPacketSize - 4; 
	TInt fragSize = 0; // size of fragment
	HBufC8 * pBuffer = NULL;
	
	// index keeps track of indexes in the buffer
	index = aStartIndex; 
	// length of data packetized, the code decrements this after each fragment is made
	length = aSize;	
	
	while ( length > 0 )
	    {
	     // Actually should be based on (PacketizationMode == INTERLEAVED && fragsPacketized == 0)
		TBool fuB = EFalse;
		
		headerByte = aBuffer[aStartIndex]  & ( 0x07 << 5 ); // Extracting F and NRI bits
		
		// taking lower 5 type bits and putting into fragHeader
		fragHeaderByte = aBuffer[aStartIndex] & 0x1f; 
			
		if ( fragsPacketized == 0 )
		    {
		    fragHeaderByte |= (0x1 << 7); // setting start bit
		    }
		    
		if ( length <= fragMaxSize )
		    {
			fragHeaderByte |= ( 0x1 << 6 );	// setting end byte
			aMarkerBit = 1;	
		    }
		else
		    {
			aMarkerBit = 0;
		    }
		
		if ( fragsPacketized == 0 )	// skipping payload header byte for FU packets 
		    {	
		    index += 1;
			length -= 1;	
		    }
	    
	    fragSize = ( length > fragMaxSize ) ? fragMaxSize+2 : length+2; // 2 bytes for headers
		
		if( !fuB )
		    {
			headerByte |= PACKET_FU_A;	
		    }
		else
		    {
			fragSize += 2; // for additional DON field
			headerByte |= PACKET_FU_B;
		    }
		
		// allocating memory for fragmented NAL unit
	    pBuffer = HBufC8::NewLC(fragSize);
	     	
 		TPtr8 pDes = pBuffer->Des(); //new (ELeave) TBuf8<size>;
		pDes.Append( &headerByte, 1 ); // appending FU-A packet header byte
		pDes.Append( &fragHeaderByte, 1 ); // appending Fragment header
		
		if ( fuB )
		    {  
		    // writing DON in network byte order
			TUint16 val = ByteOrder::Swap16( aDON );
			TUint8* ptrByte = reinterpret_cast<TUint8*>( &val );
			pDes.Append( ptrByte, 2 );
		    }
		
		TPtr8 pStart = aBuffer.MidTPtr( index ); // pStart contains the data pointer    		
    	
    	if ( !fuB )
    	    {
    		pDes.Append( pStart.Ptr(), fragSize-2 ); // copying data	
    		index += Min( length, fragMaxSize );    	
    		length -= ( fragSize-2 );
        	}
    	else
    	    {
    	    // copying data, subtracting DON and header size from total size to copy
    		pDes.Append( pStart.Ptr( ), fragSize-4 );
			index += Min( length, fragMaxSize );
			length -= ( fragSize-4 );    		
    	    }
    
    	
    	// inserting into the payloadized NAL unit buffer for retreival
    	iPayloadizedBuffers.InsertL( pBuffer, iNalCount );
    	iNalCount++;
    	CleanupStack::Pop( pBuffer );
    	pBuffer = NULL; // ownership transferred
    	
    	fragsPacketized++; // to count the number of fragments
	
	    } // end while()
	
	aNalCount = iNalCount;
    }


HBufC8 * CRFC3984Encode::GetNalUnitsInOrder( TInt & aIndex )
    {
	HBufC8 *pBuffer;
	if ( aIndex < iNalCount )
	    {
		pBuffer = iPayloadizedBuffers[aIndex];
		return (pBuffer);
	    }
	else
	    {
		return NULL;
	    }
	
    }

void CRFC3984Encode::ClearNalBuffers()
    {
	TInt count = 0;
	HBufC8 * pBuffer = NULL;
	
	for ( count = 0; count < iPayloadizedBuffers.Count(); count++ )
	    {
		pBuffer = iPayloadizedBuffers[count];
		delete pBuffer;
		pBuffer = NULL;
		iPayloadizedBuffers[count] = NULL;
	    }
	
	iPayloadizedBuffers.Reset();
	iNalCount = 0; 
    }

void CRFC3984Encode::AddSnaluPacketL( TPtr8 aStart, TInt aSize )
    {
    HBufC8* pBuffer = HBufC8::NewLC( aSize );
	
	TPtr8 pPtr = pBuffer->Des();
	pPtr.Copy( aStart.Ptr(), aSize );
	
	// inserting into the payloadized NAL unit buffer for retreival
	iPayloadizedBuffers.InsertL( pBuffer, iNalCount );
	iNalCount++;
	CleanupStack::Pop( pBuffer );
    }
    	
// End of file

