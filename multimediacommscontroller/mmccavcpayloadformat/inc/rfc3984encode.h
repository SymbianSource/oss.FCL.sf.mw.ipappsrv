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
* Description:    Class definition for rfc3984encode.h, the payloadization class
**				 for the AVC codec
**
*/




#ifndef RFC3984ENCODE_H
#define RFC3984ENCODE_H

#include <e32cmn.h>
#include <e32des8.h>
#include "rfc3984base.h"


class CRFC3984Encode : public CRFC3984Base
{
	public:
	
		// public functions
	
		~CRFC3984Encode();
		static CRFC3984Encode* NewL();
		void SetMTUSize(TUint32 aMtuSize);
		void SetFrameRate(TInt aFrameRate);
		void PayloadizeFrameL(TDes8 & aBuffer, TUint32 aTimeStamp, TUint32 & aMarkerBit, TInt &aNalCount);	
		HBufC8 * GetNalUnitsInOrder(TInt & aIndex);
		void ClearNalBuffers();
	
	protected:
	
		CRFC3984Encode();
		void PayloadizeFrameSingleNALModeL(TDes8 & aBuffer, TUint32 aTimeStamp, TUint32 & aMarkerBit, TInt & aNalCount);
		void PayloadizeFrameNonInterleavedModeL(TDes8 & aBuffer, TUint32 aTimeStamp, TUint32 & aMarkerBit, TInt & aNalCount);
		void PayloadizeNaluL(TDes8 & aBuffer, TUint32 aTimeStamp, TUint32 & aMarkerBit, TInt & aNalCount);
		void FragmentNaluL(TDes8 & aBuffer, TUint32 aTimeStamp, TUint32 & aMarkerBit, TInt & aNalCount, TInt aStartIndex, TInt aSize, TUint16 aDON);
		void AddSnaluPacketL( TPtr8 aStart, TInt aSize );
		
	private:
	
		// private members
		TInt iMaxPacketSize;				// MTU Size of the network, Used in Non-Interleaved and Interleaved mode for fragmentation
	    TInt iFrameRate;					// Frame Rate at which video is being sent, used to calculate timestamping increments
	    
	    RPointerArray<HBufC8> iPayloadizedBuffers;  // Array to keep pointers to payloadized buffers
	    TInt iNalCount;							   // count of packetized NAL units
	   
	   	// local processing members
	   	TInt iBufferIndex;					// Index to browse the buffer data, for finding start codes   
		
		RArray<TInt> iToPayloadizeBuffer;
		RArray<TInt> iToPayloadizeSizeBuffer;
		TInt		 iToPayloadizeCount;
	   	

};

#endif

// EOF