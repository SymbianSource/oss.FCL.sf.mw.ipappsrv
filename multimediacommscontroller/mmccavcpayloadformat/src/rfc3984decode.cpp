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
* Description:    AVC De-Payloadization class
*
*/




// ============================ INCLUDES =======================================

#include"rfc3984decode.h"

// ============================ MEMBER FUNCTIONS ===============================
_LIT8(AVCPSC1, "\x0\x0\x0\x1");

// -----------------------------------------------------------------------------
// CRFC3984Decode::CRFC3984Decode()
// Default Constructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRFC3984Decode::CRFC3984Decode()
{
		iNalCount = 0;
		iVirtualDON = 0;
		
}

// -----------------------------------------------------------------------------
// CRFC3984Decode::~CRFC3984Decode()
// Default Destructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRFC3984Decode::~CRFC3984Decode()
{
	// TODO :: cleanup iBuffer in this destructor
}

// -----------------------------------------------------------------------------
// CRFC3984Decode::NewL()
// First stage constructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

CRFC3984Decode * CRFC3984Decode::NewL()
{
	CRFC3984Decode * self = new(ELeave) CRFC3984Decode;
	CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop();
    return self;		
}

// -----------------------------------------------------------------------------
// CRFC3984Decode::ConstructL()
// Initializing common members
// -----------------------------------------------------------------------------
//

void CRFC3984Decode::ConstructL()
{
	iNalCount = 0;
	iVirtualDON = 0;
}

// -----------------------------------------------------------------------------
// CRFC3984Decode::DePayloadizeFrame()
// Purpose     : This function receives the encoded frame and payloadizes it based on the packetization_mode value
// Parameters  : aBuffer - Buffer containing the received NAL unit
//				 aTimeStamp - Timestamp to go into RTP
// 				 aMarkerBit - Marker Bit indication (this function sets it for key frames)
//				 aNalCount - Returns the NAL count through this reference
// Return Value: ETrue if correct parsing done, EFalse if an error occured
// -----------------------------------------------------------------------------
//

TBool CRFC3984Decode::DePayloadizeFrameL(TDes8 & aBuffer, TUint aTimeStamp, TUint aArrivalTime, TUint16 aSeqNo, TUint8 & aMarkerBit, TInt & aNalCount)
{
	
	TInt index = 0;
	CAvcRtpStruct * pRtpStruct = NULL;
	HBufC8 * pBuffer = NULL;
	TInt16	lowerSeqNo = aSeqNo;
	
	
	TUint8 header = aBuffer[index];
	TUint8 type = header & 0x1f;
	
	if(type <= PACKET_NAL_UNIT){		// SNALU packet -- add as they are
		// storing reference as it is
		pRtpStruct = new (ELeave) CAvcRtpStruct;			// allocating memory to store RTP header information and data pointer
		pRtpStruct->iMarkerBit = aMarkerBit;
		pRtpStruct->iArrivalTime = aArrivalTime;
		pRtpStruct->iTimeStamp = aTimeStamp;
		pRtpStruct->iSeqNo = aSeqNo;
		pRtpStruct->iPacketType = type;
		pRtpStruct->iDon = (iVirtualDON++)%65535; 		// Since 1 NAL/slice, no need for virtual DON and keeping it constant.
		
		CleanupStack::PushL( pRtpStruct );					// pushing to stack for leave operation	
		//pDes = new (ELeave) TBuf8<aBuffer.Size());    		// allocating memory for data
    	pBuffer = HBufC8::New(aBuffer.Size());
    	if(!pBuffer)
    	{
    		User::Leave(KErrNoMemory);
    	}
    	CleanupStack::Pop();
    	TPtr8 pDes = pBuffer->Des( );					
    	pDes.Copy(aBuffer);									// copying data
    	pRtpStruct->iDes = pBuffer;			
    	pBuffer = NULL;
    	iBuffers.Insert(pRtpStruct, iNalCount);				// storing pointer into the pointer array for retreival later on
    	iNalCount += 1;										// incrementing total NAL count by 1
	}   
	else if(type == PACKET_STAP_A){		// STAP-A packet -- seperate and add using a virtual don number
		TUint accumSize = 0;
		TUint16 nalSize = 0;	
		
		accumSize += 1; 		// incrementing to point ahead of header
		while(accumSize != aBuffer.Size()){
			nalSize = Read16(aBuffer, accumSize);
			accumSize += 2;
			if(nalSize > 0 && nalSize <= aBuffer.Size()){			// valid size, copy data
				// storing reference as it is
				pRtpStruct = new (ELeave) CAvcRtpStruct;			// allocating memory to store RTP header information and data pointer
				pRtpStruct->iMarkerBit = aMarkerBit;
				pRtpStruct->iArrivalTime = aArrivalTime;
				pRtpStruct->iTimeStamp = aTimeStamp;
				pRtpStruct->iSeqNo = aSeqNo;
				pRtpStruct->iPacketType = type;	
				pRtpStruct->iDon = (iVirtualDON++)%65535;  			// virtual DON, wrapping around at 65K (it spans over many NAL units)
				
				CleanupStack::PushL( pRtpStruct );					// pushing to stack for leave operation	
			//	pDes = new (ELeave) TBuf8<aBuffer.Size());    		// allocating memory for data
    			//pBuffer = HBufC8::New(aBuffer.Size());
    			pBuffer = HBufC8::New(nalSize);
    			if(!pBuffer)
    			{
    				User::Leave(KErrNoMemory);
    			}
    			CleanupStack::Pop();
    			TPtr8 pDes1 = pBuffer->Des( );
    			TPtr8 pStart = aBuffer.MidTPtr(accumSize); 
    			pDes1.Copy(pStart.Ptr(), nalSize);					// copying data
    			pRtpStruct->iDes = pBuffer;			
    			pBuffer = NULL;
    			iBuffers.Insert(pRtpStruct, iNalCount);				// storing pointer into the pointer array for retreival later on
    			iNalCount += 1;										// incrementing total NAL count by 1
			
				accumSize += nalSize;
			
			}
		}
	}
	else if(type == PACKET_FU_A){		// FU-A packet -- add as they are, the jitter buffer removal side will assemble them
		// storing reference as it is, since FU-A and FU-B will be depacketized at removal time from the jitter buffer
		// however, for FU-B extract the DON number
		
		// reading from FU-header byte the start bit
		index++;	// going to FU-Header Byte start bit, end bit and R bit
		TUint8 startBit = 0;
		const TUint8 * FUHeader = NULL;
		TUint8 FUHeader1 = 0;
		TInt   bufSize = aBuffer.Size();
		startBit = aBuffer[index];
		startBit &= 0x80;   // keeping just the start bit
		startBit = startBit >> 7;
		if(startBit == 1)
		{
			pBuffer = HBufC8::New(aBuffer.Size( )+3);	// excluding FU NAL Header byte and adding size of AVCPSC1
			if(!pBuffer)
    		{
				User::Leave(KErrNoMemory);
    		}
			
			TPtr8 pDesBuf = pBuffer->Des();
			TPtr8 headerPtr = aBuffer.MidTPtr(index);
			FUHeader = headerPtr.Ptr();
			FUHeader1 = *FUHeader;
			header &= 0xE0;		// making lower bits 0 i.e. type field 
			header |= (*FUHeader & 0x1F);		// now header has the actual NAL unit header values.
			pDesBuf.Append(AVCPSC1);				// appending start code
			pDesBuf.Append(&header, 1);				// appending reconstructed header
			TPtr8 startPtr = aBuffer.MidTPtr(index+1);	// getting pointer to start of NAL unit
			pDesBuf.Append(startPtr.Ptr(), bufSize-2);	// copying NAL data
		}
		else
		{
			
			pBuffer = HBufC8::New(aBuffer.Size( )-1);	// excluding FU NAL Header byte
			if(!pBuffer)
    		{
				User::Leave(KErrNoMemory);
    		}
    		
    		TPtr8 pDesBuf = pBuffer->Des();
    		TPtr8 startPtr = aBuffer.MidTPtr(index);	// pointing to FUHeader byte
    		FUHeader = startPtr.Ptr();
    		FUHeader1 = *FUHeader;
    		startPtr = aBuffer.MidTPtr(index+1);  // pointing to start of actual data
    		pDesBuf.Append(startPtr.Ptr( ), bufSize-2); // copying FU-Header and data
		}
		
		pRtpStruct = new (ELeave) CAvcRtpStruct;			// allocating memory to store RTP header information and data pointer
		pRtpStruct->iMarkerBit = aMarkerBit;
		pRtpStruct->iArrivalTime = aArrivalTime;
		pRtpStruct->iTimeStamp = aTimeStamp;
		pRtpStruct->iSeqNo = aSeqNo;
		pRtpStruct->iPacketType = type;
		pRtpStruct->iDon = (iVirtualDON++)%65535;			// keeping virtual DON for FU-A as zero too
		pRtpStruct->iDes = pBuffer;			
    	pRtpStruct->iFUHeader = FUHeader1;				// index currently pointing to FU-Header Byte
    	pBuffer = NULL;
    	iBuffers.Insert(pRtpStruct, iNalCount);				// storing pointer into the pointer array for retreival later on
    	iNalCount += 1;						
	}
	
	aNalCount = iNalCount;
	return ETrue;
}

// -----------------------------------------------------------------------------
// CRFC3984Decode::Write16()
// Purpose     : This function writes the 16 bit value in network byte order in buffer aDes (appends it to the next location)
// Parameters  : aDes - Buffer to which value has to be written
//				 aValue - Value to be written
// Return Value: 
// -----------------------------------------------------------------------------
//

void CRFC3984Decode::Write16(TDes8 & aDes, TUint16 aValue)
{
	TUint8 firstByte = (aValue & 0xFF00) >> 8;
	TUint8 secondByte = (aValue & 0xFF);
	aDes.Append(&firstByte, 1);
	aDes.Append(&secondByte, 1);	
	
}

// -----------------------------------------------------------------------------
// CRFC3984Decode::Read16()
// Purpose     : This function reads the 16 bit value in network byte order in buffer aDes (appends it to the next location)
// Parameters  : aDes - Buffer to read from
//				 aIndex - Buffer index where to read from
// Return Value: Returns the 16-bit value
// -----------------------------------------------------------------------------
//

TUint16 CRFC3984Decode::Read16(TDes8 & aDes, TInt aIndex)
{
	TUint16 value;
	
	value = (aDes[aIndex+1]) + (aDes[aIndex] << 8);
	
	return value;	
}

// -----------------------------------------------------------------------------
// CRFC3984Decode::GetNalUnit(TInt aIndex)
// Purpose     : This function returns the depayloadized NAL unit with RTP header information
// Parameters  : aIndex - Index of the entry
//				 
// Return Value: Returns a pointer to the CAvcRtpStruct structure.
// -----------------------------------------------------------------------------
//

CAvcRtpStruct * CRFC3984Decode::GetNalUnit(TInt aIndex)
{
	CAvcRtpStruct * ptr;
	if(aIndex >= iNalCount)
		return 0;
	
	ptr = iBuffers[aIndex];
	return ptr;
}


void CRFC3984Decode::ClearBuffer()
{
	if(iNalCount == 0)
		return;
		
	TInt count = 0;
	CAvcRtpStruct * ptr = NULL;
	for(count = 0; count < iNalCount; count++){
		ptr = iBuffers[count];
	
		if(ptr->iDes)	delete ptr->iDes;
		if(ptr)			delete ptr;
		ptr = NULL;
		
	}
	iBuffers.Reset( );
	iNalCount = 0;
}


// EOF