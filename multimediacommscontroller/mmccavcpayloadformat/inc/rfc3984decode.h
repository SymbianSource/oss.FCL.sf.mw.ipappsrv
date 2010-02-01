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
* Description:    Class definition for rfc3984Decode class, the de-payloadization 
**				 class for the AVC codec
**
*/




#ifndef RFC3984DECODE_H
#define RFC3984DECODE_H

//#include <e32cons.h>
//#include <e32cmn.h>
#include "rfc3984base.h"

class CRFC3984Decode : public CRFC3984Base
{
	public:
	
		// public functions
		~CRFC3984Decode();
		static CRFC3984Decode* NewL();
		TBool DePayloadizeFrameL(TDes8 & aBuffer, TUint aTimeStamp, TUint aArrivalTime, TUint16 aSeqNo, TUint8 & aMarkerBit, TInt & aNalCount);
		CAvcRtpStruct * GetNalUnit(TInt aIndex);
		void ClearBuffer();
		
	
	protected:
	
		CRFC3984Decode();	
		void ConstructL();
		void Write16(TDes8 & aDes, TUint16 aValue);
		TUint16 Read16(TDes8 & aDes, TInt aIndex);
	
	
	private:	
	
		// private members
		
		TInt iNalCount; 						// variable to keep track of Nal units depacketized in one go
		RPointerArray<CAvcRtpStruct> iBuffers;  // Array to keep pointers to depayloadized data	
		TInt iVirtualDON;
	    

};


#endif

// EOF